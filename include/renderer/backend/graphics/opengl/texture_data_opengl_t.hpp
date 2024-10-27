#pragma once

#include <string>
#include <cstdint>

#include <renderer/engine/graphics/enums.hpp>

namespace renderer {
namespace opengl {

/// Returns the given format's associated OpenGL type enum
RENDERER_API auto ToOpenGLEnum(eTextureFormat format) -> uint32_t;

/// Returns the corresponding OpenGL enum for a given eStorageType
RENDERER_API auto ToOpenGLEnum(eStorageType dtype) -> uint32_t;

/// Texture Data object (represents generally a texture's image data)
class RENDERER_API OpenGLTextureData {
    // cppcheck-suppress unknownMacro
    DEFINE_SMART_POINTERS(OpenGLTextureData)

    NO_COPY_NO_MOVE_NO_ASSIGN(OpenGLTextureData)

 public:
    /// Creates a texture-data object from a given image
    explicit OpenGLTextureData(const char* image_path);

    /// Creates a texture data object from given size and data
    explicit OpenGLTextureData(int32_t width, int32_t height, int32_t channels,
                               const uint8_t* data);

    /// Releases all allocated resources
    ~OpenGLTextureData() = default;

    RENDERER_NODISCARD auto width() const -> int32_t { return m_Width; }

    RENDERER_NODISCARD auto height() const -> int32_t { return m_Height; }

    RENDERER_NODISCARD auto channels() const -> int32_t { return m_Channels; }

    RENDERER_NODISCARD auto image_path() const -> std::string {
        return m_ImagePath;
    }

    RENDERER_NODISCARD auto format() const -> eTextureFormat {
        return m_Format;
    }

    RENDERER_NODISCARD auto storage() const -> eStorageType {
        return m_Storage;
    }

    RENDERER_NODISCARD auto data() -> uint8_t* { return m_Data.get(); }

    RENDERER_NODISCARD auto data() const -> const uint8_t* {
        return m_Data.get();
    }

    RENDERER_NODISCARD auto ToString() const -> std::string;

 private:
    /// Width of the texture image
    int32_t m_Width{0};
    /// Height of the texture image
    int32_t m_Height{0};
    /// Channels (depth) of the texture image (if applicable)
    int32_t m_Channels{0};
    /// Data type used for storage of a pixel
    eStorageType m_Storage{eStorageType::UINT_8};
    /// Format of this texture data
    eTextureFormat m_Format{eTextureFormat::RGB};
    /// Path to the resource associated with this object (if applicable)
    std::string m_ImagePath{};
    /// Buffer for the memory used by this object's texture data
    std::unique_ptr<uint8_t[]> m_Data{nullptr};  // NOLINT
};

}  // namespace opengl

}  // namespace renderer
