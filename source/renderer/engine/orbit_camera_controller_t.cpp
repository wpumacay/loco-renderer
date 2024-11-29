#include <algorithm>
#include <cmath>
#include <string>
#include <utility>

#include <spdlog/fmt/bundled/format.h>

#include <renderer/engine/buttons.hpp>
#include <renderer/engine/graphics/enums.hpp>
#include <renderer/engine/orbit_camera_controller_t.hpp>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

namespace renderer {

OrbitCameraController::OrbitCameraController(Camera::ptr camera,
                                             float viewportWidth,
                                             float viewportHeight)
    : ICameraController(std::move(camera)),
      m_ViewportWidth(viewportWidth),
      m_ViewportHeight(viewportHeight) {
    m_Type = eCameraController::ORBIT;
}

auto OrbitCameraController::Update(float dt) -> void {
    if (m_Camera == nullptr) {
        return;
    }
    constexpr auto TWO_PI = static_cast<float>(math::PI);

    auto pos_offset = m_Camera->pose.position - target;
    Vec3 target_offset;

    m_Spherical.SetFromCartesian(pos_offset);

    if (enableAutoRotate && m_State == eOrbitState::IDLE) {
        const auto AUTO_ROTATE_ANGLE = TWO_PI / 60.0F / 60.0F * autoRotateSpeed;
        m_SphericalDelta.theta -= AUTO_ROTATE_ANGLE;
    }

    if (enableDamping) {
        m_Spherical.theta += m_SphericalDelta.theta * dampingFactor;
        m_Spherical.phi += m_SphericalDelta.phi * dampingFactor;
    } else {
        m_Spherical.theta += m_SphericalDelta.theta;
        m_Spherical.phi += m_SphericalDelta.phi;
    }

    auto min = minAzimuth;
    auto max = maxAzimuth;
    if (std::isfinite(min) || std::isfinite(max)) {
        // Make sure (min, max) are in the range [-PI, PI]
        min = (min < -PI) ? (min + TWO_PI) : min;
        min = (min > PI) ? (min - TWO_PI) : min;
        max = (max < -PI) ? (max + TWO_PI) : max;
        max = (max > PI) ? (max - TWO_PI) : max;

        // By updating the (min, max) we might have flipped the order, so take
        // this into account when updating the azimuthal angle
        if (min <= max) {
            m_Spherical.theta = std::max(min, std::min(max, m_Spherical.theta));
        } else {
            m_Spherical.theta = (m_Spherical.theta > (min + max) / 2.0F)
                                    ? std::max(min, m_Spherical.theta)
                                    : std::min(max, m_Spherical.theta);
        }
    }
    m_Spherical.phi = std::max(minPolar, std::min(maxPolar, m_Spherical.phi));
    m_Spherical.MakeSafe();

    m_Spherical.rho *= m_Scale;
    m_Spherical.rho =
        std::max(minDistance, std::min(maxDistance, m_Spherical.rho));

    pos_offset = m_Spherical.GetCartesian();
    target_offset = (enableDamping)
                        ? static_cast<double>(dampingFactor) * m_PanOffset
                        : m_PanOffset;

    this->target = this->target + target_offset;
    m_Camera->target = this->target;
    m_Camera->pose.position = this->target + pos_offset;
    m_Camera->LookAt(this->target);

    if (enableDamping) {
        m_SphericalDelta.theta *= (1.0F - dampingFactor);
        m_SphericalDelta.phi *= (1.0F - dampingFactor);
        m_PanOffset = m_PanOffset * static_cast<double>(1.0F - dampingFactor);
    } else {
        m_SphericalDelta.rho = 0.0F;
        m_SphericalDelta.phi = 0.0F;
        m_SphericalDelta.theta = 0.0F;
        m_PanOffset = {0.0F, 0.0F, 0.0F};
    }
    m_Scale = 1.0F;
}

auto OrbitCameraController::OnMouseButtonCallback(int button, int action,
                                                  double x, double y) -> void {
    if (m_Camera == nullptr) {
        return;
    }

    if (!enabled) {
        return;
    }

    if (action == button_action::BUTTON_RELEASED) {
        m_State = eOrbitState::IDLE;
    } else if (action == button_action::BUTTON_PRESSED) {
        switch (button) {
            case mouse::BUTTON_LEFT: {
                if (!enableRotate) {
                    return;
                }

                m_RotateStart.x() = static_cast<float>(x);
                m_RotateStart.y() = static_cast<float>(y);
                m_State = eOrbitState::ROTATE;
                break;
            }
            case mouse::BUTTON_RIGHT: {
                if (!enablePan) {
                    return;
                }

                m_PanStart.x() = static_cast<float>(x);
                m_PanStart.y() = static_cast<float>(y);
                m_State = eOrbitState::PAN;
                break;
            }
            case mouse::BUTTON_MIDDLE: {
                if (!enableZoom) {
                    return;
                }

                m_DollyStart.x() = static_cast<float>(x);
                m_DollyStart.y() = static_cast<float>(y);
                m_State = eOrbitState::DOLLY;
                break;
            }
        }
    }
}

auto OrbitCameraController::OnMouseMoveCallback(double x, double y) -> void {
    if (m_Camera == nullptr) {
        return;
    }

    if (!enabled) {
        return;
    }

    switch (m_State) {
        case eOrbitState::ROTATE: {
            if (!enableRotate) {
                return;
            }

            constexpr auto TWO_PI = static_cast<float>(math::PI);
            m_RotateCurrent.x() = static_cast<float>(x);
            m_RotateCurrent.y() = static_cast<float>(y);
            m_RotateDelta = (m_RotateCurrent - m_RotateStart) *
                            static_cast<double>(rotateSpeed);

            m_SphericalDelta.theta -=
                (TWO_PI * m_RotateDelta.x() / m_ViewportWidth);
            m_SphericalDelta.phi -=
                (TWO_PI * m_RotateDelta.y() / m_ViewportHeight);

            m_RotateStart = m_RotateCurrent;
            Update(0.0F);
            break;
        }
        case eOrbitState::PAN: {
            if (!enablePan) {
                return;
            }

            m_PanCurrent.x() = static_cast<float>(x);
            m_PanCurrent.y() = static_cast<float>(y);
            m_PanDelta =
                (m_PanCurrent - m_PanStart) * static_cast<double>(panSpeed);

            switch (m_Camera->data.projection) {
                case eProjectionType::PERSPECTIVE: {
                    auto offset = m_Camera->pose.position - target;
                    auto target_distance = math::norm(offset);
                    const auto FOV = m_Camera->data.fov;
                    target_distance *= std::tan((FOV / 2.0F) * PI / 180.0F);
                    auto pan_horizontal_dist = 2.0F * m_PanDelta.x() *
                                               target_distance /
                                               m_ViewportHeight;
                    auto pan_vertical_dist = 2.0F * m_PanDelta.y() *
                                             target_distance / m_ViewportHeight;

                    Vec3 diff_horizontal =
                        static_cast<double>(-pan_horizontal_dist) *
                        m_Camera->v_right;

                    Vec3 diff_vertical;
                    if (screenSpacePanning) {
                        diff_vertical = static_cast<double>(pan_vertical_dist) *
                                        m_Camera->v_up;
                    } else {
                        diff_vertical =
                            static_cast<double>(pan_vertical_dist) *
                            math::normalize(math::cross(m_Camera->worldUp,
                                                        m_Camera->v_right));
                    }

                    m_PanOffset = m_PanOffset + diff_horizontal + diff_vertical;
                    break;
                }
                case eProjectionType::ORTHOGRAPHIC:
                    auto pan_horizontal_dist = m_PanDelta.x() *
                                               m_Camera->data.width /
                                               m_Camera->zoom / m_ViewportWidth;
                    auto pan_vertical_dist = m_PanDelta.y() *
                                             m_Camera->data.height /
                                             m_Camera->zoom / m_ViewportHeight;

                    // TODO(wilbert): refactor duplicated code here and above
                    Vec3 diff_horizontal =
                        static_cast<double>(-pan_horizontal_dist) *
                        m_Camera->v_right;

                    Vec3 diff_vertical;
                    if (screenSpacePanning) {
                        diff_vertical = static_cast<double>(pan_vertical_dist) *
                                        m_Camera->v_up;
                    } else {
                        diff_vertical =
                            static_cast<double>(pan_vertical_dist) *
                            math::normalize(math::cross(m_Camera->worldUp,
                                                        m_Camera->v_right));
                    }

                    m_PanOffset = m_PanOffset + diff_horizontal + diff_vertical;
                    break;
            }

            m_PanStart = m_PanCurrent;
            Update(0.0F);
            break;
        }
        case eOrbitState::DOLLY: {
            if (!enableZoom) {
                return;
            }

            m_DollyCurrent.x() = static_cast<float>(x);
            m_DollyCurrent.y() = static_cast<float>(y);
            m_DollyDelta = m_DollyCurrent - m_DollyStart;

            _HandleDolly(m_DollyDelta.y());

            m_DollyStart = m_DollyCurrent;
            Update(0.0F);
            break;
        }
        default:
            break;
    }
}

auto OrbitCameraController::OnScrollCallback(double xOff, double yOff) -> void {
    _HandleDolly(-static_cast<float>(yOff));
}

auto OrbitCameraController::OnResizeCallback(int width, int height) -> void {
    m_ViewportWidth = static_cast<float>(width);
    m_ViewportHeight = static_cast<float>(height);
}

auto OrbitCameraController::_HandleDolly(float movement) -> void {
    if (m_Camera == nullptr) {
        return;
    }

    const auto ZOOM_SCALE = std::pow(0.95F, zoomSpeed);
    const auto PROJ_TYPE = m_Camera->data.projection;
    if (movement > 0.0F) {
        // Dolly out
        switch (PROJ_TYPE) {
            case eProjectionType::PERSPECTIVE: {
                m_Scale /= ZOOM_SCALE;
                break;
            }
            case eProjectionType::ORTHOGRAPHIC: {
                m_Camera->zoom = std::max(
                    minZoom, std::min(maxZoom, m_Camera->zoom * ZOOM_SCALE));
                break;
            }
        }
    } else if (movement < 0.0F) {
        // Dolly in
        switch (PROJ_TYPE) {
            case eProjectionType::PERSPECTIVE: {
                m_Scale *= ZOOM_SCALE;
                break;
            }
            case eProjectionType::ORTHOGRAPHIC: {
                m_Camera->zoom = std::max(
                    minZoom, std::min(maxZoom, m_Camera->zoom / ZOOM_SCALE));
                break;
            }
        }
    }
}

auto OrbitCameraController::ToString() const -> std::string {
    auto camera_name = (m_Camera != nullptr ? m_Camera->name() : "None");
    return fmt::format(
        "<OrbitCameraController\n"
        "  camera: {0}\n"
        "  enabled: {1}\n"
        "  state: {2}\n"
        "  target: {3}\n"
        "  sphere.rho: {4}\n"
        "  sphere.phi: {5}\n"
        "  sphere.theta: {6}\n"
        ">\n",
        camera_name, this->enabled, ::renderer::ToString(m_State),
        this->target.toString(), m_Spherical.rho, m_Spherical.phi,
        m_Spherical.theta);
}

}  // namespace renderer

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
