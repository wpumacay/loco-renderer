#include <string>

#include <spdlog/fmt/bundled/format.h>

#include <renderer/engine/input_manager_t.hpp>

namespace renderer {

auto InputManager::CallbackKey(int key, int action) -> void {
    m_Keys.at(static_cast<size_t>(key)) = action;
}

auto InputManager::CallbackMouseButton(int button, int action, float x, float y)
    -> void {
    m_Buttons.at(static_cast<size_t>(button)) = action;
    m_Cursor.x() = x;
    m_Cursor.y() = y;
}

auto InputManager::CallbackMouseMove(float x, float y) -> void {
    m_Cursor.x() = x;
    m_Cursor.y() = y;
}

auto InputManager::CallbackScroll(float xOff, float yOff) -> void {
    m_ScrollOff.x() = xOff;
    m_ScrollOff.y() = yOff;
    m_ScrollAccum.x() += xOff;
    m_ScrollAccum.y() += yOff;
}

auto InputManager::IsKeyDown(int key) const -> bool {
    if (key < 0 || key > MAX_KEYS) {
        return false;
    }

    return (m_Keys.at(static_cast<size_t>(key)) == key_action::PRESSED) ||
           (m_Keys.at(static_cast<size_t>(key)) == key_action::REPEAT);
}

auto InputManager::IsMouseDown(int button) const -> bool {
    if (button < 0 || button > MAX_BUTTONS) {
        return false;
    }

    return m_Buttons.at(static_cast<size_t>(button)) ==
           button_action::BUTTON_PRESSED;
}

auto InputManager::ToString() const -> std::string {
    auto str_repr = fmt::format(
        "<InputManager\n"
        "  cursor: {0}\n"
        "  scrollOff: {1}\n"
        "  scrollAccum: {2}\n",
        m_Cursor.toString(), m_ScrollOff.toString(), m_ScrollAccum.toString());
    str_repr += "  keysDown: ";
    for (int key = 0; key < MAX_KEYS; ++key) {
        if (IsKeyDown(key)) {
            str_repr += std::to_string(key) + " - ";
        }
    }
    str_repr += "\n";
    str_repr += ">\n";

    return str_repr;
}

}  // namespace renderer
