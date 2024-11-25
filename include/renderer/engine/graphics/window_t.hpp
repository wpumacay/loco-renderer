#pragma once

#include <memory>
#include <string>

#include <renderer/common.hpp>
#include <renderer/engine/callbacks.hpp>
#include <renderer/engine/keycodes.hpp>
#include <renderer/engine/graphics/window_config_t.hpp>
#include <renderer/engine/graphics/window_adapter_t.hpp>

namespace renderer {

/// Abstraction of a window supporting multiple windowing backends
class RENDERER_API Window : public std::enable_shared_from_this<Window> {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Window)

    DEFINE_SMART_POINTERS(Window)

 public:
    /// Creates a window from a given set of configuration options
    explicit Window(WindowConfig config);

    /// Creates a window given its width, height, and (optionally) backend
    explicit Window(int width, int height,
                    eWindowBackend backend = eWindowBackend::TYPE_GLFW);

    ~Window() = default;

    /// Creates a window from some options
    static auto CreateWindow(int width, int height, eWindowBackend backend)
        -> std::shared_ptr<Window> {
        auto window = std::make_shared<Window>(width, height, backend);
        window->Initialize();
        return window;
    }

    /// Creates a window from a given set of configuration options
    static auto CreateWindow(WindowConfig win_cfg) -> std::shared_ptr<Window> {
        auto window = std::make_shared<Window>(win_cfg);
        window->Initialize();
        return window;
    }

    /// Initializes the window and backend-related resources
    auto Initialize() -> void;

    /// Requests (if applicable) enabling the cursor on this window
    auto EnableCursor() -> void;

    /// Requests (if applicable) disabling the cursor on this window
    auto DisableCursor() -> void;

    /// Prepares the window for receiving rendering commands
    auto Begin() -> void;

    /// Cleans up and setup the window after all rendering has been done
    auto End() -> void;

    /// Requests (if applicable) the window to close itself in the next frame
    auto RequestClose() -> void;

    /// Sets the background color of the window
    auto SetClearColor(const Vec4& color) -> void;

    /// Registers (if applicable) a callback to be called on keyboard events
    auto RegisterKeyboardCallback(const KeyboardCallback& callback) -> void;

    /// Registers (if applicable) a callback to be called on mouse-down events
    auto RegisterMouseButtonCallback(const MouseButtonCallback& callback)
        -> void;

    /// Registers (if applicable) a callback to be called on mouse-move events
    auto RegisterMouseMoveCallback(const MouseMoveCallback& callback) -> void;

    /// Registers (if applicable) a callback to called on mouse-scroll events
    auto RegisterScrollCallback(const ScrollCallback& callback) -> void;

    /// Registers (if applicable) a callback to be called on resize-window
    /// events
    auto RegisterResizeCallback(const ResizeCallback& callback) -> void;

    /// Returns whether or not this window is still active
    RENDERER_NODISCARD auto active() const -> bool { return m_Active; }

    /// Returns an unmutable reference to the clear-color property
    RENDERER_NODISCARD auto clear_color() const -> Vec4 {
        return m_Config.clear_color;
    }

    /// Returns the current width (in pixels) of this window
    RENDERER_NODISCARD auto width() const -> int { return m_Config.width; }

    /// Returns the current height (in pixels) of this window
    RENDERER_NODISCARD auto height() const -> int { return m_Config.height; }

    /// Returns the title of this window
    RENDERER_NODISCARD auto title() const -> std::string {
        return m_Config.title;
    }

    /// Returns the identifier of the windowing-backend used for this window
    RENDERER_NODISCARD auto backend() const -> eWindowBackend {
        return m_Config.backend;
    }

    /// Returns an unmutable reference to the config of this window
    RENDERER_NODISCARD auto config() const -> const WindowConfig& {
        return m_Config;
    }

 private:
    /// Creates the internal adapter to link to the specific windowing backend
    auto _InitializeBackend() -> void;

 private:
    /// The configuration used for creating this window
    WindowConfig m_Config;

    /// Whether or not the window is still active
    bool m_Active = false;

    /// Adapter used to link to a specific windowing backend
    std::unique_ptr<IWindowAdapter> m_BackendAdapter = nullptr;
};

}  // namespace renderer
