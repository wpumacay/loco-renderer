#include <algorithm>
#include <string>
#include <utility>

#include <spdlog/fmt/bundled/format.h>

#include <renderer/engine/buttons.hpp>
#include <renderer/engine/keycodes.hpp>
#include <renderer/engine/fps_camera_controller_t.hpp>

#include <utils/logging.hpp>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

namespace renderer {

FpsCameraController::FpsCameraController(Camera::ptr camera)
    : ICameraController(std::move(camera)) {
    m_Type = eCameraController::FPS;
}

auto FpsCameraController::Update(float dt) -> void {
    if (m_Camera == nullptr) {
        return;
    }

    constexpr auto MAX_DT = 0.1F;  // Max delta allowed is 100ms
    dt = std::min(MAX_DT, dt);

    Vec3 velocity = {0.0F, 0.0F, 0.0F};
    Vec3 direction = {0.0F, 0.0F, 0.0F};

    velocity.x() -= velocity.x() * 10.0F * dt;
    velocity.z() -= velocity.z() * 10.0F * dt;

    direction.z() =
        (m_MoveForward ? 1.0F : 0.0F) - (m_MoveBackward ? 1.0F : 0.0F);
    direction.x() = (m_MoveRight ? 1.0F : 0.0F) - (m_MoveLeft ? 1.0F : 0.0F);

    if (m_MoveForward || m_MoveBackward) {
        velocity.z() -= direction.z() * 400.0F * dt;
    }
    if (m_MoveLeft || m_MoveRight) {
        velocity.x() -= direction.x() * 400.0F * dt;
    }

    auto dfront = m_Camera->v_front * static_cast<double>(velocity.z() * dt);
    auto dright = m_Camera->v_right * (static_cast<double>(-velocity.x() * dt));
    auto dtotal = dfront + dright;

    m_Camera->target = m_Camera->target + dtotal;
    m_Camera->pose.position = m_Camera->pose.position + dtotal;
    // Update the orientation (changes in pitch and yaw)
    // TODO(wilbert): implement remaining updates

    LOG_CORE_TRACE("position: {0}", m_Camera->pose.position.toString());

    m_Camera->LookAt(m_Camera->target);
}

auto FpsCameraController::OnKeyCallback(int key, int action,
                                        int modifier) -> void {
    if (m_Camera == nullptr) {
        return;
    }

    if (!enabled) {
        return;
    }

    if (action == key_action::PRESSED || action == key_action::REPEAT) {
        switch (key) {
            case keys::KEY_UP:
            case keys::KEY_W: {
                m_MoveForward = true;
                break;
            }

            case keys::KEY_DOWN:
            case keys::KEY_S: {
                m_MoveBackward = true;
                break;
            }

            case keys::KEY_LEFT:
            case keys::KEY_A: {
                m_MoveLeft = true;
                break;
            }

            case keys::KEY_RIGHT:
            case keys::KEY_D: {
                m_MoveRight = true;
                break;
            }
        }
    } else if (action == key_action::RELEASED) {
        switch (key) {
            case keys::KEY_UP:
            case keys::KEY_W: {
                m_MoveForward = false;
                break;
            }
            case keys::KEY_DOWN:
            case keys::KEY_S: {
                m_MoveBackward = false;
                break;
            }
            case keys::KEY_LEFT:
            case keys::KEY_A: {
                m_MoveLeft = false;
                break;
            }
            case keys::KEY_RIGHT:
            case keys::KEY_D: {
                m_MoveRight = false;
                break;
            }
        }
    }
}

auto FpsCameraController::OnMouseMoveCallback(double x, double y) -> void {
    if (m_Camera == nullptr) {
        return;
    }

    if (!enabled) {
        return;
    }

    constexpr float MAX_DX = 50.0F;
    constexpr float MAX_DY = 50.0F;
    auto dx = std::min(MAX_DX, static_cast<float>(x) - m_LastCursor.x());
    auto dy = std::min(MAX_DY, static_cast<float>(y) - m_LastCursor.y());
    m_LastCursor.x() = static_cast<float>(x);
    m_LastCursor.y() = static_cast<float>(y);

    Euler euler;
    euler.order = ::math::euler::Order::ZXY;
    euler.convention = ::math::euler::Convention::INTRINSIC;
    euler.setFromQuaternion(m_Camera->pose.orientation);

    euler.x -= dy * 0.002F * this->pointerSpeed;
    euler.y -= dx * 0.002F * this->pointerSpeed;

    m_Camera->LookAt(Quat(euler));
}

auto FpsCameraController::ToString() const -> std::string {
    auto camera_name = (m_Camera != nullptr ? m_Camera->name() : "None");
    return fmt::format(
        "<FpsCameraController\n"
        "  camera: {0}\n"
        "  enabled: {1}\n"
        "  moveForward: {2}\n"
        "  moveBackward: {3}\n"
        "  moveRight: {4}\n"
        "  moveLeft: {5}\n"
        ">\n",
        camera_name, this->enabled);
}

}  // namespace renderer

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#endif
