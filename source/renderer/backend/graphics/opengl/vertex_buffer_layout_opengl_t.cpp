#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <spdlog/fmt/bundled/format.h>

#include <renderer/backend/graphics/opengl/vertex_buffer_layout_opengl_t.hpp>

namespace renderer {
namespace opengl {

auto OpenGLBufferElement::ToString() const -> std::string {
    return fmt::format(
        "<OpenGLBufferElement\n"
        "  name: {0}\n"
        "  type: {1}\n"
        "  count: {2}\n"
        "  nbytes: {3}\n"
        "  offset: {4}\n"
        "  normalized: {5}\n"
        ">\n",
        name, ::renderer::ToString(type), count, nbytes, offset, normalized);
}

OpenGLBufferLayout::OpenGLBufferLayout(
    const std::initializer_list<OpenGLBufferElement>& elements)
    : m_BufferElements(elements) {
    for (auto& element : m_BufferElements) {
        element.offset = m_Stride;
        m_Stride += element.nbytes;
    }
}

OpenGLBufferLayout::OpenGLBufferLayout(
    std::vector<OpenGLBufferElement> elements)
    : m_BufferElements(std::move(elements)) {
    for (auto& element : m_BufferElements) {
        element.offset = m_Stride;
        m_Stride += element.nbytes;
    }
}

auto OpenGLBufferLayout::AddElement(OpenGLBufferElement element) -> void {
    element.offset = m_Stride;
    m_Stride += element.nbytes;
    m_BufferElements.push_back(std::move(element));
}

auto OpenGLBufferLayout::operator[](size_t index) const -> OpenGLBufferElement {
    if (index >= m_BufferElements.size()) {
        throw std::runtime_error(
            fmt::format("OpenGLBufferLayout> index {0} out of range [0-{1})",
                        index, m_BufferElements.size()));
    }
    return m_BufferElements[index];
}

auto OpenGLBufferLayout::ToString() const -> std::string {
    std::string str_repr{"<OpenGLBufferLayout({\n"};
    for (const auto& element : m_BufferElements) {
        str_repr += fmt::format(
            "  [name={0}, type={1}, count={2}, nbytes={3}, offset={4}, "
            "normalized={5}]\n",
            element.name, renderer::ToString(element.type), element.count,
            element.nbytes, element.offset, element.normalized);
    }
    str_repr += "})>\n";
    return str_repr;
}

}  // namespace opengl

}  // namespace renderer
