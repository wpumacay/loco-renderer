#pragma once

#include <array>
#include <string>

#include <renderer/common.hpp>
#include <renderer/engine/keycodes.hpp>
#include <renderer/engine/buttons.hpp>

namespace renderer {

class RENDERER_API InputManager {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(InputManager)

    DEFINE_SMART_POINTERS(InputManager)

 public:
    InputManager() = default;

    ~InputManager() = default;

    /// Handler for key events
    auto CallbackKey(int key, int action) -> void;

    /// Handler for mouse events
    auto CallbackMouseButton(int button, int action, float x, float y) -> void;

    /// Handler for mouse-move events
    auto CallbackMouseMove(float x, float y) -> void;

    /// Handler for scroll events
    auto CallbackScroll(float xOff, float yOff) -> void;

    /// Returns whether or not the given key is being pressed
    RENDERER_NODISCARD auto IsKeyDown(int key) const -> bool;

    /// Returns whether or not the given mouse button is being pressed
    RENDERER_NODISCARD auto IsMouseDown(int button) const -> bool;

    /// Returns the current position of the cursor
    RENDERER_NODISCARD auto GetCursorPosition() const -> Vec2 {
        return m_Cursor;
    }

    /// Returns the scroll offset for the corresponding x-axis of your device
    RENDERER_NODISCARD auto GetScrollOffX() const -> float {
        return m_ScrollOff.x();
    }

    /// Returns the scroll offset for the corresponding y-axis of your device
    RENDERER_NODISCARD auto GetScrollOffY() const -> float {
        return m_ScrollOff.y();
    }

    /// Returns the acccumulated scroll for the corresponding x-axis of your dev
    RENDERER_NODISCARD auto GetScrollAccumX() const -> float {
        return m_ScrollAccum.x();
    }

    /// Returns the acccumulated scroll for the corresponding y-axis of your dev
    RENDERER_NODISCARD auto GetScrollAccumY() const -> float {
        return m_ScrollAccum.y();
    }

    /// Returns the string representation of the input manager
    RENDERER_NODISCARD auto ToString() const -> std::string;

 private:
    /// Buffer to keep which keys are pressed and which are not
    std::array<int, MAX_KEYS> m_Keys{};
    /// Buffer to keep which mouse buttons are pressed and which are not
    std::array<int, MAX_BUTTONS> m_Buttons{};
    /// Current position of the mouse cursor
    Vec2 m_Cursor;
    /// Current value of the scroll wheel
    Vec2 m_ScrollOff;
    /// Accumulated value of the scroll wheel
    Vec2 m_ScrollAccum;
};

}  // namespace renderer
