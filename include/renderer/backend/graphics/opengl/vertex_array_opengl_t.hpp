#pragma once

#include <vector>
#include <string>

#include <renderer/common.hpp>
#include <renderer/backend/graphics/opengl/vertex_buffer_opengl_t.hpp>
#include <renderer/backend/graphics/opengl/index_buffer_opengl_t.hpp>

namespace renderer {
namespace opengl {

/// Returns the appropriate GL enum for the given etype
RENDERER_NODISCARD auto ToOpenGLEnum(eElementType etype) -> uint32_t;

/// Vertex Array Object (VAO), used to handle vertex attribs
class RENDERER_API OpenGLVertexArray {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(OpenGLVertexArray)

    DEFINE_SMART_POINTERS(OpenGLVertexArray)

 public:
    /// Creates a VAO on the GPU
    OpenGLVertexArray();

    /// Releases the resources allocated  and owned by this VAO
    ~OpenGLVertexArray();

    /// Adds the given VBO to the group managed by this VAO
    auto AddVertexBuffer(OpenGLVertexBuffer::ptr buffer) -> void;

    /// Adds the given IBO to the group managed by this VAO
    auto SetIndexBuffer(OpenGLIndexBuffer::ptr ibuffer) -> void;

    /// Binds this VAO, setting the OpenGL-FSM to the appropriate state
    auto Bind() const -> void;

    /// Unbinds this VAO from the OpenGL-FSM
    auto Unbind() const -> void;

    /// Returns an unmutable reference to the VBO at the given index
    RENDERER_NODISCARD auto GetVertexBuffer(uint32_t vbo_index) const
        -> const OpenGLVertexBuffer&;

    /// Returns a mutable reference to the VBO at the given index
    RENDERER_NODISCARD auto GetVertexBuffer(uint32_t vbo_index)
        -> OpenGLVertexBuffer&;

    /// Returns the id associated with this object on the GPU
    RENDERER_NODISCARD auto opengl_id() const -> uint32_t { return m_OpenGLId; }

    ///  Returns the current number of attributes managed by the VAO
    RENDERER_NODISCARD auto num_attribs() const -> uint32_t {
        return m_NumAttribIndx;
    }

    /// Returns the number of VBOs managed by this object
    RENDERER_NODISCARD auto num_vbos() const -> size_t {
        return m_Buffers.size();
    }

    /// Returns a string representation of this VAO
    RENDERER_NODISCARD auto ToString() const -> std::string;

 private:
    /// Id of the OpenGL resource allocated on the GPU
    uint32_t m_OpenGLId{0};

    /// Index of the current slot in the attributePointer list of the VAO
    uint32_t m_NumAttribIndx{0};

    /// Container for the owned VBOs
    std::vector<OpenGLVertexBuffer::ptr> m_Buffers;

    /// IBO associated with this VAO (if applicable)
    OpenGLIndexBuffer::ptr m_IndexBuffer{nullptr};
};

}  // namespace opengl

}  // namespace renderer
