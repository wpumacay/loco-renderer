#pragma once

#include <string>
#include <unordered_map>

#include <renderer/common.hpp>
#include <renderer/engine/graphics/buffer_attribute_t.hpp>

namespace renderer {

class RENDERER_API Geometry {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Geometry)

    DEFINE_SMART_POINTERS(Geometry)
 public:
    Geometry() = default;

    /// Creates a geometry with the given vertex data
    /// \param[in] n_vertices The number of vertices to be stored
    /// \param[in] v_positions The vertex data associated with the positions
    /// \param[in] v_normals The vertex data associated with the normals
    /// \param[in] v_texcoords The vertex data associated with the UVs
    explicit Geometry(size_t n_vertices, const float32_t* v_positions,
                      const float32_t* v_normals, const float32_t* v_texcoords);

    /// Creates a geometry with the given vertex and indices data
    /// \param[in] n_vertices The number of vertices to be stored
    /// \param[in] v_positions The vertex data associated with the positions
    /// \param[in] v_normals The vertex data associated with the normals
    /// \param[in] v_texcoords The vertex data associated with the UVs
    /// \param[in] n_indices The number of indices to be stored
    /// \param[in] indices A pointer to the data for the indices
    explicit Geometry(size_t n_vertices, const float32_t* v_positions,
                      const float32_t* v_normals, const float32_t* v_texcoords,
                      size_t n_indices, const uint32_t* indices);

    ~Geometry() = default;

    /// Creates an attribute with given data and stores it in this geometry
    /// \param[in] attrib_name The name of the attribute (as saved in geometry)
    /// \param[in] attrib_type The type of elements stored by the attribute
    /// \param[in] attrib_num The number of elements stored by the attribute
    /// \param[in] attrib_data A pointer to the data for the new attribute
    /// \param[in] attrib_normalized Whether or not the attribute is normalized
    auto AddAttribute(std::string attrib_name, eElementType attrib_type,
                      size_t attrib_num, const float32_t* attrib_data,
                      bool attrib_normalized) -> void;

    /// Creates an attribute for the indices stored for this geometry
    /// \param[in] n_indices The number of indices to be stored
    /// \param[in] data A pointer to the
    auto SetIndices(size_t n_indices, const uint32_t* data) -> void;

    /// Returns whether or not the attribute with given name exists
    RENDERER_NODISCARD auto HasAttribute(const std::string& name) const -> bool;

    /// Returns an unmutable reference to an attribute, or throws if not found
    RENDERER_NODISCARD auto GetAttribute(const std::string& name) const
        -> const Float32BufferAttribute&;

    /// Returns the number of vertices
    RENDERER_NODISCARD auto num_vertices() const -> size_t {
        return m_NumVertices;
    }

 public:
    /// Storage for buffer attributes
    std::unordered_map<std::string, Float32BufferAttribute::uptr> attributes;

    /// Buffer attribute used for storing indices
    Uint32BufferAttribute::uptr indices{nullptr};

 protected:
    /// The number of vertices stored
    size_t m_NumVertices{0};
};

}  // namespace renderer
