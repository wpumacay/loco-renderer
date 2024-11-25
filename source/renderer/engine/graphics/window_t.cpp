#include <renderer/engine/graphics/window_t.hpp>
#include <renderer/engine/graphics/window_adapter_t.hpp>
#include <renderer/backend/window/window_adapter_glfw.hpp>
// #include <renderer/backend/window/window_adapter_egl.hpp>

namespace renderer {

Window::Window(WindowConfig config) : m_Config(std::move(config)) {}

Window::Window(int width, int height, eWindowBackend backend) {
    m_Config.width = width;
    m_Config.height = height;
    m_Config.backend = backend;
}

auto Window::Initialize() -> void {
    _InitializeBackend();
}

auto Window::_InitializeBackend() -> void {
    switch (m_Config.backend) {
        case eWindowBackend::TYPE_GLFW:
            m_BackendAdapter = std::make_unique<WindowAdapterGLFW>(m_Config);
            break;
        // case eWindowBackend::TYPE_EGL:
        //     m_BackendAdapter = std::make_unique<WindowAdapterEGL>(m_Config);
        //     break;
        default:
            // TODO(wilbert): use WindowAdapterNone here
            break;
    }
    m_Active = true;

    if (m_BackendAdapter) {
        m_BackendAdapter->SetWindowHandle(shared_from_this());
    }
}

auto Window::EnableCursor() -> void {
    if (m_BackendAdapter) {
        m_BackendAdapter->EnableCursor();
    }
}

auto Window::DisableCursor() -> void {
    if (m_BackendAdapter) {
        m_BackendAdapter->DisableCursor();
    }
}

auto Window::Begin() -> void {
    if (m_BackendAdapter) {
        m_BackendAdapter->Begin();
    }
}

auto Window::End() -> void {
    if (m_BackendAdapter) {
        m_BackendAdapter->End();
    }
}

auto Window::RequestClose() -> void {
    if (m_BackendAdapter) {
        m_BackendAdapter->RequestClose();
    }
    m_Active = false;
}

auto Window::SetClearColor(const Vec4& color) -> void {
    if (m_BackendAdapter) {
        m_BackendAdapter->SetClearColor(color);
    }
    m_Config.clear_color = color;
}

auto Window::RegisterKeyboardCallback(const KeyboardCallback& callback)
    -> void {
    if (m_BackendAdapter) {
        m_BackendAdapter->RegisterKeyboardCallback(callback);
    }
}

auto Window::RegisterMouseButtonCallback(const MouseButtonCallback& callback)
    -> void {
    if (m_BackendAdapter) {
        m_BackendAdapter->RegisterMouseButtonCallback(callback);
    }
}

auto Window::RegisterMouseMoveCallback(const MouseMoveCallback& callback)
    -> void {
    if (m_BackendAdapter) {
        m_BackendAdapter->RegisterMouseMoveCallback(callback);
    }
}

auto Window::RegisterScrollCallback(const ScrollCallback& callback) -> void {
    if (m_BackendAdapter) {
        m_BackendAdapter->RegisterScrollCallback(callback);
    }
}

auto Window::RegisterResizeCallback(const ResizeCallback& callback) -> void {
    if (m_BackendAdapter) {
        m_BackendAdapter->RegisterResizeCallback(callback);
    }
}

}  // namespace renderer
