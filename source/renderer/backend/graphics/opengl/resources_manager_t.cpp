#include <memory>
#include <string>

#include <spdlog/fmt/bundled/format.h>

#include <utils/logging.hpp>

#include <renderer/backend/graphics/opengl/resources_manager_t.hpp>

namespace renderer {
namespace opengl {

auto ResourcesManager::LoadProgram(const std::string& name,
                                   const std::string& vertex_filepath,
                                   const std::string& fragment_filepath)
    -> OpenGLProgram::ptr {
    if (m_Programs.find(name) != m_Programs.end()) {
        LOG_CORE_WARN(
            "ResourcesManager::LoadProgram > tried to load duplicate program "
            "\"{0}\"",
            name);
        return m_Programs[name];
    }

    auto vert_src = ::utils::GetFileContents(vertex_filepath.c_str());
    auto frag_src = ::utils::GetFileContents(fragment_filepath.c_str());

    auto program =
        std::make_shared<OpenGLProgram>(vert_src.c_str(), frag_src.c_str());

    m_Programs[name] = program;
    return program;
}

auto ResourcesManager::LoadTexture(const std::string& name,
                                   const std::string& filepath)
    -> OpenGLTexture::ptr {
    if (m_Textures.find(name) != m_Textures.end()) {
        LOG_CORE_WARN(
            "ResourcesManager::LoadTexture > tried to load duplicate texture "
            "\"{0}\"",
            name);
        return m_Textures[name];
    }

    auto texture = std::make_shared<OpenGLTexture>(filepath.c_str());

    m_Textures[name] = texture;
    return texture;
}

auto ResourcesManager::GetProgram(const std::string& name)
    -> OpenGLProgram::ptr {
    if (m_Programs.find(name) != m_Programs.end()) {
        return m_Programs[name];
    }

    LOG_CORE_WARN(
        "ResourcesManager::GetProgram > program \"{0}\"not found in cache",
        name);
    return nullptr;
}

auto ResourcesManager::GetTexture(const std::string& name)
    -> OpenGLTexture::ptr {
    if (m_Textures.find(name) != m_Textures.end()) {
        return m_Textures[name];
    }

    LOG_CORE_WARN(
        "ResourcesManager::GetTexture > texture \"{0}\"not found in cache",
        name);
    return nullptr;
}

auto ResourcesManager::ToString() const -> std::string {
    std::string str_repr =
        "<ResourcesManager\n"
        "  programs: [\n";
    for (const auto& kv : m_Programs) {
        str_repr += "\t" + kv.first + ",\n";
    }
    str_repr += "  ]\n";
    str_repr += "  textures: [\n";
    for (const auto& kv : m_Textures) {
        str_repr += "\t" + kv.first + ",\n";
    }
    str_repr += "  ]\n";
    str_repr += ">\n";

    return str_repr;
}

}  // namespace opengl
}  // namespace renderer
