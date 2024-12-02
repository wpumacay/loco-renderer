#include <memory>
#include <string>

#include <utils/logging.hpp>

#include <spdlog/fmt/bundled/format.h>

#include <renderer/backend/graphics/opengl/renderer_opengl_t.hpp>

namespace renderer {
namespace opengl {

OpenGLRenderer::OpenGLRenderer() {
    m_ResourcesManager = std::make_unique<ResourcesManager>();
}

auto OpenGLRenderer::DrawLine(Vec3 start, Vec3 end, Vec3 color) -> void {
    /// TODO(wilbert): implement using debug drawer
    LOG_CORE_TRACE(
        "DrawLine(\n"
        "  start: {0}\n"
        "  end: {1}\n"
        "  color: {2}\n"
        ")\n",
        start.toString(), end.toString(), color.toString());
}

auto OpenGLRenderer::Render(Scene::ptr scene, Camera::ptr camera) -> void {
    /// TODO(wilbert): implement rendering pipeline
    LOG_CORE_TRACE(
        "Render(\n"
        "  scene: {0}\n"
        "  camera: {1}\n"
        ")\n",
        scene->name(), camera->name());
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
