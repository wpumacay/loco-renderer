#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>

#include <glad/gl.h>

#include <spdlog/fmt/bundled/format.h>

#include <renderer/engine/graphics/enums.hpp>
#include <renderer/backend/graphics/opengl/vertex_array_opengl_t.hpp>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif

namespace renderer {
namespace opengl {

auto ToOpenGLEnum(eElementType etype) -> uint32_t {
    switch (etype) {
        case eElementType::FLOAT_1:
        case eElementType::FLOAT_2:
        case eElementType::FLOAT_3:
        case eElementType::FLOAT_4:
            return GL_FLOAT;
        case eElementType::INT_1:
        case eElementType::INT_2:
        case eElementType::INT_3:
        case eElementType::INT_4:
            return GL_INT;
        default:
            return GL_FLOAT;
    }
}

OpenGLVertexArray::OpenGLVertexArray() { glGenVertexArrays(1, &m_OpenGLId); }

OpenGLVertexArray::~OpenGLVertexArray() {
    m_Buffers.clear();
    if (m_OpenGLId != 0) {
        glDeleteVertexArrays(1, &m_OpenGLId);
        m_OpenGLId = 0;
    }
}

auto OpenGLVertexArray::AddVertexBuffer(OpenGLVertexBuffer::ptr buffer)
    -> void {
    const auto& buffer_layout = buffer->layout();

    const auto STRIDE = buffer_layout.stride();

    glBindVertexArray(m_OpenGLId);
    glBindBuffer(GL_ARRAY_BUFFER, buffer->opengl_id());

    for (size_t i = 0; i < buffer_layout.size(); ++i) {
        const auto& element = buffer_layout[i];
        glEnableVertexAttribArray(m_NumAttribIndx);
        glVertexAttribPointer(m_NumAttribIndx, static_cast<int>(element.count),
                              ToOpenGLEnum(element.type),
                              element.normalized ? GL_TRUE : GL_FALSE,
                              static_cast<int>(STRIDE),
                              // cppcheck-suppress cstyleCast
                              (const void*)(intptr_t)element.offset);  // NOLINT
        m_NumAttribIndx++;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_Buffers.push_back(std::move(buffer));
}

auto OpenGLVertexArray::SetIndexBuffer(OpenGLIndexBuffer::ptr ibuffer) -> void {
    glBindVertexArray(m_OpenGLId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer->opengl_id());
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_IndexBuffer = std::move(ibuffer);
}

auto OpenGLVertexArray::Bind() const -> void { glBindVertexArray(m_OpenGLId); }

// NOLINTNEXTLINE
auto OpenGLVertexArray::Unbind() const -> void { glBindVertexArray(0); }

auto OpenGLVertexArray::GetVertexBuffer(uint32_t vbo_index) const
    -> const OpenGLVertexBuffer& {
    if (vbo_index >= m_Buffers.size()) {
        auto message = fmt::format("Index {0} out of range {1}", vbo_index,
                                   m_Buffers.size());
        throw std::runtime_error(message);
    }
    return *m_Buffers[vbo_index];
}

auto OpenGLVertexArray::GetVertexBuffer(uint32_t vbo_index)
    -> OpenGLVertexBuffer& {
    if (vbo_index >= m_Buffers.size()) {
        auto message = fmt::format("Index {0} out of range {1}", vbo_index,
                                   m_Buffers.size());
        throw std::runtime_error(message);
    }
    return *m_Buffers[vbo_index];
}

auto OpenGLVertexArray::ToString() const -> std::string {
    return fmt::format(
        "<OpenGLVertexArray\n"
        "  num-attribs: {0}\n"
        "  num-buffers: {1}\n"
        "  opengl-id: {2}\n"
        ">\n",
        m_NumAttribIndx, m_Buffers.size(), m_OpenGLId);
}

}  // namespace opengl

}  // namespace renderer

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
