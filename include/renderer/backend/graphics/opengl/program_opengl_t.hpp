#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include <renderer/common.hpp>
#include <renderer/engine/graphics/enums.hpp>

namespace renderer {
namespace opengl {

/// Returns the respective OpenGL enum for the given shader type
auto ToOpenGLEnum(eShaderType type) -> uint32_t;

/// Compiles a shader given its source and shader type
auto CompileShader(const char* source, eShaderType type) -> uint32_t;

/// Shader program class, used to handle an OpenGL Shader Program in GPU
class RENDERER_API OpenGLProgram {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(OpenGLProgram)

    DEFINE_SMART_POINTERS(OpenGLProgram)

 public:
    /// Creates a shader-program object with given name and source code
    /// \param[in] vert_src Source code of the vertex-shader
    /// \param[in] frag_src Source code of the fragment-shader
    explicit OpenGLProgram(const char* vert_src, const char* frag_src);

    /// Releases the resources allocated for this Shader Program on GPU
    ~OpenGLProgram();

    /// Links all the shaders associated with this program
    auto Build() -> void;

    /// Binds the current program for usage in the rendering pipeline
    auto Bind() const -> void;

    /// Unbinds the current program from the rendering pipeline
    auto Unbind() const -> void;

    /// Sets an int32 uniform given its name and desired value
    auto SetInt(const char* uname, int32_t uvalue) -> void;

    /// Sets a float32 uniform given its name and desired value
    auto SetFloat(const char* uname, float uvalue) -> void;

    /// Sets a vec-2 uniform given its name and desired value
    auto SetVec2(const char* uname, const Vec2& uvalue) -> void;

    /// Sets a vec-3 unbiform given its name and desired value
    auto SetVec3(const char* uname, const Vec3& uvalue) -> void;

    /// Sets a vec-4 unbiform given its name and desired value
    auto SetVec4(const char* uname, const Vec4& uvalue) -> void;

    /// Sets a mat-4 unbiform given its name and desired value
    auto SetMat4(const char* uname, const Mat4& uvalue) -> void;

    /// Returns the string representation
    RENDERER_NODISCARD auto ToString() const -> std::string;

    /// Returns whether or not this shader is valid
    RENDERER_NODISCARD auto IsValid() const -> bool { return m_IsValid; }

    /// Returns the code used for the vertex shader stage
    RENDERER_NODISCARD auto vertex_source() const -> std::string {
        return m_VertSource;
    }

    /// Returns the code used for the fragment shader stage
    RENDERER_NODISCARD auto fragment_source() const -> std::string {
        return m_FragSource;
    }

 private:
    /// Caches and returns the requested uniform location
    auto _GetUniformLocation(const char* uname) -> int32_t;

 private:
    /// Source code for the vertex shader stage
    std::string m_VertSource;

    /// Source code for the fragment shader stage
    std::string m_FragSource;

    // THe OpenGL ID associated to this program
    uint32_t m_OpenGLId{0};

    /// Whether or not the shader program was build successfully
    bool m_IsValid{false};

    /// Map used to keep uniforms' names and their locations
    std::unordered_map<std::string, int32_t> m_UniformLocationsCache;
};

}  // namespace opengl

}  // namespace renderer
