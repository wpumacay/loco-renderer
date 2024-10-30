#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <renderer/backend/graphics/opengl/program_opengl_t.hpp>
#include <renderer/backend/graphics/opengl/texture_opengl_t.hpp>

namespace renderer {
namespace opengl {

class RENDERER_API ResourcesManager {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(ResourcesManager)

    DEFINE_SMART_POINTERS(ResourcesManager)
 public:
    ResourcesManager() = default;

    ~ResourcesManager() = default;

    auto LoadProgram(const std::string& name,
                     const std::string& vertex_filepath,
                     const std::string& fragment_filepath)
        -> OpenGLProgram::ptr;

    auto LoadTexture(const std::string& name, const std::string& filepath)
        -> OpenGLTexture::ptr;

    auto GetProgram(const std::string& name) -> OpenGLProgram::ptr;

    auto GetTexture(const std::string& name) -> OpenGLTexture::ptr;

    RENDERER_NODISCARD auto ToString() const -> std::string;

 private:
    /// Container for shader programs managed by this manager
    std::unordered_map<std::string, OpenGLProgram::ptr> m_Programs;

    /// Container for textures loaded by this manager
    std::unordered_map<std::string, OpenGLTexture::ptr> m_Textures;
};

}  // namespace opengl
}  // namespace renderer
