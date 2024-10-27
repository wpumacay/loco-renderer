#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <renderer/engine/graphics/enums.hpp>
#include <renderer/backend/graphics/opengl/texture_data_opengl_t.hpp>

namespace renderer {
namespace opengl {

/// Returns the associated OpenGL enum for the given texture wrapping mode
RENDERER_API auto ToOpenGLEnum(eTextureWrap tex_wrap) -> int32_t;

/// Returns the associated OpenGL enum for this given texture filter options
RENDERER_API auto ToOpenGLEnum(eTextureFilter tex_filter) -> int32_t;

/// Returns the associated OpenGL enum of the given internal format type
RENDERER_API auto ToOpenGLEnum(eTextureIntFormat tex_iformat) -> int32_t;

class RENDERER_API OpenGLTexture {
    // cppcheck-suppress unknownMacro
    DEFINE_SMART_POINTERS(OpenGLTexture)

    NO_COPY_NO_MOVE_NO_ASSIGN(OpenGLTexture)

 public:
    explicit OpenGLTexture(const char* image_path);

    explicit OpenGLTexture(OpenGLTextureData::ptr tex_data);

    ~OpenGLTexture();

    auto Bind() const -> void;

    auto Unbind() const -> void;

    auto SetBorderColor(const Vec4& color) -> void;

    auto SetMinFilter(const eTextureFilter& tex_filter) -> void;

    auto SetMagFilter(const eTextureFilter& tex_filter) -> void;

    auto SetWrapModeU(const eTextureWrap& tex_wrap) -> void;

    auto SetWrapModeV(const eTextureWrap& tex_wrap) -> void;

    RENDERER_NODISCARD auto opengl_id() const -> uint32_t { return m_OpenGLId; }

    RENDERER_NODISCARD auto border_color() const -> Vec4 {
        return m_BorderColor;
    }

    RENDERER_NODISCARD auto internal_format() const -> eTextureIntFormat {
        return m_IntFormat;
    }

    RENDERER_NODISCARD auto min_filter() const -> eTextureFilter {
        return m_MinFilter;
    }

    RENDERER_NODISCARD auto mag_filter() const -> eTextureFilter {
        return m_MagFilter;
    }

    RENDERER_NODISCARD auto wrap_mode_u() const -> eTextureWrap {
        return m_WrapU;
    }

    RENDERER_NODISCARD auto wrap_mode_v() const -> eTextureWrap {
        return m_WrapV;
    }

    RENDERER_NODISCARD auto texture_data() -> OpenGLTextureData::ptr {
        return m_TextureData;
    }

    RENDERER_NODISCARD auto ToString() const -> std::string;

 private:
    /// Initializes the texture
    auto _InitializeTexture() -> void;

 private:
    /// Id of the OpenGL resource allocated on the GPU
    uint32_t m_OpenGLId{0};

    /// Color used at the border (U|horizontal coordinate)
    Vec4 m_BorderColor;

    /// Internal format (number of color components of the texture). See [1]
    eTextureIntFormat m_IntFormat{eTextureIntFormat::RGB};

    /// Filter used for minification
    eTextureFilter m_MinFilter{eTextureFilter::NEAREST};

    /// Filter used for magnification
    eTextureFilter m_MagFilter{eTextureFilter::NEAREST};

    /// Wrapping mode (U|horizontal coordinate)
    eTextureWrap m_WrapU{eTextureWrap::REPEAT};

    /// Wrapping mode (V|vertical coordinate)
    eTextureWrap m_WrapV{eTextureWrap::REPEAT};

    /// OpenGLTexture data (contains the image data)
    OpenGLTextureData::ptr m_TextureData{nullptr};
};

}  // namespace opengl

}  // namespace renderer
