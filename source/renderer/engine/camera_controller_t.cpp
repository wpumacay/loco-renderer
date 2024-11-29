#include <string>

#include <spdlog/fmt/bundled/format.h>

#include <renderer/engine/camera_controller_t.hpp>

namespace renderer {

auto ICameraController::ToString() const -> std::string {
    auto camera_name = (m_Camera != nullptr ? m_Camera->name() : "None");
    auto camera_type = ::renderer::ToString(m_Type);
    // Show this information as base case
    return fmt::format(
        "<ICameraController\n"
        "  camera: {0}\n"
        "  type: {1}\n"
        "  enabled: {2}\n"
        ">\n",
        camera_name, camera_type, this->enabled);
}

}  // namespace renderer
