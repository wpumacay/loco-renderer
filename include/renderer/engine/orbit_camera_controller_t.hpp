#pragma once

// Implementation based on Three.JS OrbitControls [3]

#include <limits>
#include <string>

#include <math/utils/spherical_coordinates.hpp>
#include <renderer/engine/camera_controller_t.hpp>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

namespace renderer {

class OrbitCameraController : public ICameraController {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(OrbitCameraController)

    DEFINE_SMART_POINTERS(OrbitCameraController)

 public:
    explicit OrbitCameraController(Camera::ptr camera, float viewportWidth,
                                   float viewportHeight);

    ~OrbitCameraController() override = default;

    auto Update(float dt) -> void override;

    auto OnKeyCallback(int key, int action, int modifier) -> void override {}

    auto OnMouseButtonCallback(int button, int action, double x,
                               double y) -> void override;

    auto OnMouseMoveCallback(double x, double y) -> void override;

    auto OnScrollCallback(double xOff, double yOff) -> void override;

    auto OnResizeCallback(int width, int height) -> void override;

    RENDERER_NODISCARD auto state() const -> eOrbitState { return m_State; }

    RENDERER_NODISCARD auto ToString() const -> std::string override;

 private:
    /// Compute the scale required for zooming in and out
    auto _HandleDolly(float movement) -> void;

 public:
    /// How far can we orbit vertically (lower limit)
    float minPolar = 0.0F;
    /// How far can we orbit vertically (upper limit)
    float maxPolar = static_cast<float>(math::PI);
    /// How far can we orbit horizontally (lower limit)
    float minAzimuth = -std::numeric_limits<float>::infinity();
    /// How far can we orbit horizontally (upper limit)
    float maxAzimuth = std::numeric_limits<float>::infinity();
    /// How far can we dolly out
    float minDistance = 0;
    /// How far can we dolly in
    float maxDistance = std::numeric_limits<float>::infinity();
    /// The speed at which a rotation with the mouse can be achieved
    float rotateSpeed = 1.0F;
    /// The speed at which we can pan with the mouse
    float panSpeed = 1.0F;
    /// The speed at which we can zoom with the mouse wheel
    float zoomSpeed = 1.0F;
    /// Minimum value allowed for zomming in or out
    float minZoom = 1e-3F;
    /// Maximum value allowed for zooming in or out
    float maxZoom = std::numeric_limits<float>::infinity();
    /// The location where the object orbits around
    Vec3 target;

    /// Whether or not to enable rotation
    bool enableRotate = true;
    /// Whether or not to enable panning
    bool enablePan = true;
    /// Whether or not to enable dollying|zoom
    bool enableZoom = true;

    /// Whether or not to enable damping to the rotation movement
    bool enableDamping = false;
    /// The damping factor used for damping the rotation movement
    float dampingFactor = 0.05F;

    /// Whether or not to autorotate around the focus target
    bool enableAutoRotate = false;
    /// The speed at which the autorotate works for this controller
    float autoRotateSpeed = 2.0F;

    /// Controls the panning directions. If true, pans horizontally and
    /// vertically (following the camera's right and up vectors). If false,
    /// instead of the up direction, it follows the direction orthogonal to
    /// the camera's up vector and its right vector
    bool screenSpacePanning = true;

 private:
    /// Current state of this orbit controller
    eOrbitState m_State = eOrbitState::IDLE;

    /// Where the mouse cursor is when pressed at the start of a rotation
    Vec2 m_RotateStart;
    /// Where the mouse cursor currently is when a move move event is received
    Vec2 m_RotateCurrent;
    /// Keeps tracks of the delta movement when doing a rotation
    Vec2 m_RotateDelta;
    /// Current position in spherical coordinates
    math::SphericalCoords<float> m_Spherical;
    /// Delta between two positions in spherical coordinates
    math::SphericalCoords<float> m_SphericalDelta;

    /// Where the mouse cursor is (at the start of panning)
    Vec2 m_PanStart;
    /// Where the mouse cursor is while the mouse moves (while panning)
    Vec2 m_PanCurrent;
    /// Keeps tracks of the delta movement when panning
    Vec2 m_PanDelta;
    /// The vector we use to move the target when panning
    Vec3 m_PanOffset;

    /// Where the mouse cursor is (at the start of dollying)
    Vec2 m_DollyStart;
    /// Where the mouse cursor is while the mouse moves (while dollying)
    Vec2 m_DollyCurrent;
    /// Keeps tracks of the delta movement when dollying
    Vec2 m_DollyDelta;
    /// The scale factor to apply to the radius due to dollying
    float m_Scale = 1.0F;

    /// The width currently being used by the screen when rendering
    float m_ViewportWidth = 800.0F;
    /// The height currently being used by the screen when rendering
    float m_ViewportHeight = 600.0F;
};

}  // namespace renderer

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
