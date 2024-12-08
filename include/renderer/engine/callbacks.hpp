#pragma once

#include <cstdint>
#include <functional>

namespace renderer {

/// Whether or not the shift keys were held down
constexpr int32_t KEY_MOD_SHIFT = (1 << 0);
/// Whether or not the ctrl keys were held down
constexpr int32_t KEY_MOD_CONTROL = (1 << 1);
/// Whether or not the alt keys were held down
constexpr int32_t KEY_MOD_ALT = (1 << 2);
/// Whether or not the super key was held down
constexpr int32_t KEY_MOD_SUPER = (1 << 3);

/// Function signature for keyboard callbacks
using KeyboardCallback = std::function<void(int, int, int)>;
/// Function signature for mouse-button callbacks
using MouseButtonCallback = std::function<void(int, int, double, double)>;
/// Function signature for mouse-move callbacks
using MouseMoveCallback = std::function<void(double, double)>;
/// Function signature for mouse-scroll callbacks
using ScrollCallback = std::function<void(double, double)>;
/// Function signature for window-resize callbacks
using ResizeCallback = std::function<void(int, int)>;

}  // namespace renderer
