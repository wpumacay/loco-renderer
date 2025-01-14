#include <memory>
#include <string>

#include <utils/logging.hpp>

#include <spdlog/fmt/bundled/format.h>

#include <renderer/backend/graphics/opengl/renderer_opengl_t.hpp>

namespace renderer {
namespace opengl {

OpenGLRenderer::OpenGLRenderer() {
    m_ResourcesManager = std::make_unique<ResourcesManager>();
    m_DebugDrawer = std::make_unique<OpenGLDebugDrawer>();
}

auto OpenGLRenderer::DrawLine(Vec3 start, Vec3 end, Vec3 color) -> void {
    if (m_DebugDrawer) {
        m_DebugDrawer->DrawLine(start, end, color);
    }
}

auto OpenGLRenderer::Render(const Scene& scene, const Camera& camera) -> void {
    // Render debug primitives on top of everything else
    if (m_DebugDrawer) {
        m_DebugDrawer->Render(camera);
    }
}

auto OpenGLRenderer::ToString() const -> std::string {
    return fmt::format(
        "<OpenGLRenderer\n"
        "  numDrawcalls: {0}\n"
        ">\n",
        m_NumDrawcalls);
}

}  // namespace opengl
}  // namespace renderer
