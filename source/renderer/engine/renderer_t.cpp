#include <string>

#include <spdlog/fmt/bundled/format.h>

#include <renderer/engine/renderer_t.hpp>

namespace renderer {

auto IRenderer::ToString() const -> std::string {
    return fmt::format(
        "<IRenderer\n"
        "  enabled: {0}\n"
        "  debugEnabled: {1}\n"
        ">\n",
        m_Enabled, m_DebugEnabled);
}

}  // namespace renderer
