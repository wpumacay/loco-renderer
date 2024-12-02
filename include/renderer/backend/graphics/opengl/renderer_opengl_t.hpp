#pragma once

#include <string>

#include <renderer/engine/renderer_t.hpp>
#include <renderer/backend/graphics/opengl/resources_manager_t.hpp>

namespace renderer {
namespace opengl {

class RENDERER_API OpenGLRenderer : public ::renderer::IRenderer {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(OpenGLRenderer)

    DEFINE_SMART_POINTERS(OpenGLRenderer)
 public:
    OpenGLRenderer();

    ~OpenGLRenderer() override = default;

    auto DrawLine(Vec3 start, Vec3 end, Vec3 color) -> void override;

    auto Render(Scene::ptr scene, Camera::ptr camera) -> void override;

    /// Returns the number of drawcalls spent in a render call
    RENDERER_NODISCARD auto numDrawcalls() const -> int {
        return m_NumDrawcalls;
    }

    RENDERER_NODISCARD auto ToString() const -> std::string override;

 protected:
    /// A counter for the number of draw calls executed
    int m_NumDrawcalls{0};

    /// Resources manager to handle shaders and textures
    ResourcesManager::uptr m_ResourcesManager{nullptr};
};

}  // namespace opengl
}  // namespace renderer
