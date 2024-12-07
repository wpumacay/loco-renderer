#pragma once

#include <cassert>
#include <memory>
#include <string>

#include <renderer/common.hpp>
#include <renderer/engine/graphics/enums.hpp>

namespace renderer {

class Float32BufferAttribute {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Float32BufferAttribute)

    DEFINE_SMART_POINTERS(Float32BufferAttribute)
 public:
    /// Creates a buffer attribute given some data and properties
    /// \param[in] element_type The type of element used for this attribute
    /// \param[in] num_elements The number of elements stored by this attribute
    /// \param[in] data A pointer to the data to be copied for this attribute
    /// \param[in] normalized Whether or not the attribute should be normalized
    explicit Float32BufferAttribute(eElementType element_type,
                                    size_t num_elements, const float32_t* data,
                                    bool normalized = false);

    ~Float32BufferAttribute() = default;

    /// Updates the buffer data with some new data
    /// \param[in] num_elements The new number of elements to store
    /// \param[in] data A pointer to the data to be sotred in the buffer
    auto UpdateData(size_t num_elements, const float32_t* data) -> void;

    /// Returns an unmutable reference to the data stored by this attribute
    RENDERER_NODISCARD auto data() const -> const float32_t* {
        return m_Buffer.get();
    }

    /// Returns the float value at the given index
    RENDERER_NODISCARD auto operator[](size_t index) const -> float32_t {
        assert(index < m_NumElements);
        return m_Buffer[index];
    }

    /// Returns the size of the element representing this attribute
    RENDERER_NODISCARD auto element_size() const -> size_t {
        return m_ElementSize;
    }

    /// Returns whether or not this attribute should be normalized
    RENDERER_NODISCARD auto normalized() const -> bool { return m_Normalized; }

    /// Returns a string representation of this attribute
    RENDERER_NODISCARD auto ToString() const -> std::string;

 protected:
    /// The storage used for this buffer
    std::unique_ptr<float32_t[]> m_Buffer{nullptr};  // NOLINT

    /// The type of elements stored by this attribute
    eElementType m_ElementType{eElementType::FLOAT_3};

    /// The number of elements stored by this attribute
    size_t m_NumElements{0};

    /// The size of each item of this attribute
    size_t m_ElementSize{0};

    /// Whether or not the data for this attribute should be normalized
    bool m_Normalized{false};
};

class RENDERER_API Uint32BufferAttribute {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Uint32BufferAttribute)

    DEFINE_SMART_POINTERS(Uint32BufferAttribute)
 public:
    explicit Uint32BufferAttribute(size_t num_indices, const uint32_t* data);

    ~Uint32BufferAttribute() = default;

    RENDERER_NODISCARD auto data() const -> const uint32_t* {
        return m_Buffer.get();
    }

    RENDERER_NODISCARD auto element_size() const -> size_t {
        return m_ElementSize;
    }

    RENDERER_NODISCARD auto num_indices() const -> size_t {
        return m_NumIndices;
    }

    RENDERER_NODISCARD auto ToString() const -> std::string;

 protected:
    /// The storage used for this buffer
    std::unique_ptr<uint32_t[]> m_Buffer{nullptr};  // NOLINT

    /// The number of indices stored by this buffer
    size_t m_NumIndices{0};

    /// The size of each index stored in this buffer
    size_t m_ElementSize{0};
};

}  // namespace renderer
