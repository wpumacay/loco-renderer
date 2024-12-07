#include <memory>
#include <string>
#include <utility>

#include <spdlog/fmt/bundled/format.h>
#include <utils/logging.hpp>

#include <renderer/engine/graphics/geometry_t.hpp>

namespace renderer {

Geometry::Geometry(size_t n_vertices, const float32_t* v_positions,
                   const float32_t* v_normals, const float32_t* v_texcoords)
    : m_NumVertices(n_vertices) {
    AddAttribute("position", ::renderer::eElementType::FLOAT_3, m_NumVertices,
                 v_positions, false);
    AddAttribute("normal", ::renderer::eElementType::FLOAT_3, m_NumVertices,
                 v_normals, true);
    AddAttribute("texcoord", ::renderer::eElementType::FLOAT_2, m_NumVertices,
                 v_texcoords, false);
}

Geometry::Geometry(size_t n_vertices, const float32_t* v_positions,
                   const float32_t* v_normals, const float32_t* v_texcoords,
                   size_t n_indices, const uint32_t* indices)
    : m_NumVertices(n_vertices) {
    AddAttribute("position", ::renderer::eElementType::FLOAT_3, m_NumVertices,
                 v_positions, false);
    AddAttribute("normal", ::renderer::eElementType::FLOAT_3, m_NumVertices,
                 v_normals, true);
    AddAttribute("texcoord", ::renderer::eElementType::FLOAT_2, m_NumVertices,
                 v_texcoords, false);
    SetIndices(n_indices, indices);
}

auto Geometry::AddAttribute(std::string attrib_name, eElementType attrib_type,
                            size_t attrib_num, const float32_t* attrib_data,
                            bool attrib_normalized) -> void {
    if (attrib_num != m_NumVertices) {
        LOG_CORE_WARN(
            "Geometry::AddAttribute >>> mismatch between attribute count {0} "
            "and number of vertices {1}",
            attrib_num, m_NumVertices);
        return;
    }

    if (HasAttribute(attrib_name)) {
        LOG_CORE_WARN(
            "Geometry::AddAttribute >>> an attribute with name {0} already "
            "exists",
            attrib_name);
        return;
    }

    auto attribute = std::make_unique<Float32BufferAttribute>(
        attrib_type, attrib_num, attrib_data, attrib_normalized);
    this->attributes[std::move(attrib_name)] = std::move(attribute);
}

auto Geometry::SetIndices(size_t n_indices, const uint32_t* data) -> void {
    if (this->indices != nullptr) {
        LOG_CORE_WARN(
            "Geometry::AddAttribute >>> indices attrib already exists");
        return;
    }
    this->indices = std::make_unique<Uint32BufferAttribute>(n_indices, data);
}

auto Geometry::HasAttribute(const std::string& name) const -> bool {
    return this->attributes.find(name) != this->attributes.end();
}

auto Geometry::GetAttribute(const std::string& name) const
    -> const Float32BufferAttribute& {
    if (!HasAttribute(name)) {
        throw std::runtime_error(
            fmt::format("GetAttribute >>> attribute {0} not found", name));
    }
    return *this->attributes.at(name);
}

}  // namespace renderer
