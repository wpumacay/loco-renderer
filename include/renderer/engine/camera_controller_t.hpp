#pragma once

#include <string>
#include <utility>

#include <renderer/engine/graphics/enums.hpp>
#include <renderer/engine/camera_t.hpp>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

namespace renderer {

/// Interface for all supported camera controllers
class RENDERER_API ICameraController {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(ICameraController)

    DEFINE_SMART_POINTERS(ICameraController)

 public:
    explicit ICameraController(Camera::ptr camera)
        : m_Camera(std::move(camera)) {}

    virtual ~ICameraController() = default;

    /// Updates the current state of the controller
    virtual auto Update(float dt) -> void = 0;

    /// Called when a keyboard event is received
    virtual auto OnKeyCallback(int key, int action, int modifier) -> void = 0;

    /// Called when a mouse-button event is received
    virtual auto OnMouseButtonCallback(int button, int action, double x,
                                       double y) -> void = 0;

    /// Called when a mouse-move event is received
    virtual auto OnMouseMoveCallback(double x, double y) -> void = 0;

    /// Called when a scroll event is received
    virtual auto OnScrollCallback(double xOff, double yOff) -> void = 0;

    /// Called when a resize event is received
    virtual auto OnResizeCallback(int width, int height) -> void = 0;

    RENDERER_NODISCARD virtual auto ToString() const -> std::string;

    /// Returns the type of this controller
    RENDERER_NODISCARD auto type() const -> eCameraController { return m_Type; }

    /// Returns the camera being handled by this controller
    RENDERER_NODISCARD auto camera() const -> Camera::ptr { return m_Camera; }

 public:
    /// Whether or not this controller is enabled
    bool enabled = true;

 protected:
    /// Type of camera controller
    eCameraController m_Type = eCameraController::NONE;
    /// Camera being handled by this controller
    Camera::ptr m_Camera = nullptr;
};

/// Dummy camera controller (events are just NOPs)
class RENDERER_API DummyCameraController : public ICameraController {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(DummyCameraController)

    DEFINE_SMART_POINTERS(DummyCameraController)

 public:
    explicit DummyCameraController(Camera::ptr camera)
        : ICameraController(std::move(camera)) {}

    ~DummyCameraController() override = default;

    auto Update(float dt) -> void override {}

    auto OnKeyCallback(int key, int action, int modifier) -> void override {}

    auto OnMouseButtonCallback(int button, int action, double x,
                               double y) -> void override {}

    auto OnMouseMoveCallback(double x, double y) -> void override {}

    auto OnScrollCallback(double xOff, double yOff) -> void override {}

    auto OnResizeCallback(int width, int height) -> void override {}
};

}  // namespace renderer

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
