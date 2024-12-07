#include <cstring>
#include <string>

#include <spdlog/fmt/bundled/format.h>

#include <renderer/engine/graphics/buffer_attribute_t.hpp>

namespace renderer {

Float32BufferAttribute::Float32BufferAttribute(eElementType element_type,
                                               size_t num_elements,
                                               const float32_t* data,
                                               bool normalized)
    : m_ElementType(element_type),
      m_NumElements(num_elements),
      m_ElementSize(GetElementSize(element_type)),
      m_Normalized(normalized) {
    const auto NUM_FLOATS = m_NumElements * GetElementCount(element_type);
    m_Buffer = std::make_unique<float32_t[]>(NUM_FLOATS);  // NOLINT
    memcpy(m_Buffer.get(), data, sizeof(float32_t) * NUM_FLOATS);
}

auto Float32BufferAttribute::UpdateData(size_t num_elements,
                                        const float32_t* data) -> void {
    m_NumElements = num_elements;
    const auto NUM_FLOATS = m_NumElements * GetElementCount(m_ElementType);
    m_Buffer = std::make_unique<float32_t[]>(NUM_FLOATS);  // NOLINT
    memcpy(m_Buffer.get(), data, sizeof(float32_t) * NUM_FLOATS);
}

auto Float32BufferAttribute::ToString() const -> std::string {
    return fmt::format(
        "<Float32BufferAttribute\n"
        "  elementType: {0}\n"
        "  elementSize: {1}\n"
        "  numElements: {2}\n"
        "  normalized: {3}\n"
        ">\n",
        ::renderer::ToString(m_ElementType), m_ElementSize, m_NumElements,
        m_Normalized);
}

Uint32BufferAttribute::Uint32BufferAttribute(size_t num_indices,
                                             const uint32_t* data)
    : m_NumIndices(num_indices), m_ElementSize(sizeof(uint32_t)) {
    m_Buffer = std::make_unique<uint32_t[]>(num_indices);  // NOLINT
    memcpy(m_Buffer.get(), data, sizeof(uint32_t) * num_indices);
}

auto Uint32BufferAttribute::ToString() const -> std::string {
    return fmt::format(
        "<Uint32BufferAttribute\n"
        "  numIndices: {0}\n"
        "  elementSize: {1}\n"
        ">\n",
        m_NumIndices, m_ElementSize);
}

}  // namespace renderer
