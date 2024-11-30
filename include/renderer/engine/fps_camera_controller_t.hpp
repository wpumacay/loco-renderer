#pragma once

// Implementation based on ThreeJS PointerLockControls [4]

#include <string>

#include <renderer/engine/camera_controller_t.hpp>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

namespace renderer {

class RENDERER_API FpsCameraController : public ICameraController {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(FpsCameraController)

    DEFINE_SMART_POINTERS(FpsCameraController)

 public:
    explicit FpsCameraController(Camera::ptr camera);

    ~FpsCameraController() override = default;

    /// Called when the cursor is locked; the controller can be used and updated
    auto OnCursorLocked() -> void { this->enabled = true; }

    /// Called when the cursor is released; the controller is disabled
    auto OnCursorReleased() -> void { this->enabled = false; }

    /// Update the internal state of the
    auto Update(float dt) -> void override;

    /// Handle move logic on this callback (moving horizontally or vertically)
    auto OnKeyCallback(int key, int action, int modifier) -> void override;

    /// This controller doesn't setup nor use any resources during this callback
    auto OnMouseButtonCallback(int button, int action, double x,
                               double y) -> void override {}

    /// Handle the orientation of the camera by focusing on a target point
    auto OnMouseMoveCallback(double x, double y) -> void override;

    /// This controller doesn't setup nor use any resources during this callback
    auto OnScrollCallback(double xOff, double yOff) -> void override {}

    /// This controller doesn't setup nor use any resources during this callback
    auto OnResizeCallback(int width, int height) -> void override {}

    RENDERER_NODISCARD auto ToString() const -> std::string override;

 public:
    /// The lower limit of the pitch of the camera
    float minPolar = 0.0F;
    /// The upper limit of the pitch of the camera
    float maxPolar = PI;
    /// The speed at which we can pitch
    float pointerSpeed = 1.0F;

 private:
    /// Whether or not we're moving in the forward direction
    bool m_MoveForward = false;
    /// Whether or not we're moving in the backward direction
    bool m_MoveBackward = false;
    /// Whether or not we're moving to the right
    bool m_MoveRight = false;
    /// Whether or not we're moving to the left
    bool m_MoveLeft = false;
    /// The last position of the cursor
    Vec2 m_LastCursor;
};

}  // namespace renderer

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
