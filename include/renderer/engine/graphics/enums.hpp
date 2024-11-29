#pragma once

#include <string>
#include <renderer/common.hpp>

namespace renderer {

/// Available graphics APIs
enum class eGraphicsAPI {
    OPENGL,
    VULKAN,
    DIRECTX11,
    DIRECTX12,
};

/// Retuns the string representation of the given graphics API enum
RENDERER_API auto ToString(eGraphicsAPI api) -> std::string;

/// Available windowing backends
enum class eWindowBackend {
    /// None or dummy backend (for testing purposes only)
    TYPE_NONE,
    /// GLFW backend (used for creating a window for OpenGL in any platform)
    TYPE_GLFW,
    /// EGL backend (used for headless rendering mode)
    TYPE_EGL,
};

/// Returns the string representation of the given backend type
RENDERER_API auto ToString(eWindowBackend type) -> std::string;

/// Available shader types
enum class eShaderType {
    VERTEX,    //< Associated to a vertex shasder
    FRAGMENT,  //< Associated to a fragment shader
    GEOMETRY,  //< Associated to a geometry shader
    COMPUTE,   //< Associated to a compute shader
};

/// Returns the string representation of the given shader type
RENDERER_API auto ToString(eShaderType type) -> std::string;

/// Type of element used for part (or all) elements in a GPU vertex buffer
enum class eElementType {
    FLOAT_1,  ///< Single float, size 4-bytes
    FLOAT_2,  ///< Two float compound (vec2), size 8-bytes
    FLOAT_3,  ///< Three float compound (vec3), size 12-bytes
    FLOAT_4,  ///< Four float compound (vec4), size 16-bytes
    INT_1,    ///< Single integer, size 4-bytes (=int32)
    INT_2,    ///< Two integer compound (int2), size 8-bytes
    INT_3,    ///< Three integer compound (int3), size 12-bytes
    INT_4,    ///< Four integer compound (int4), size 16-bytes
};

/// Returns the string representation of the given element-type enum
RENDERER_API auto ToString(eElementType etype) -> std::string;

/// Returns the size (in bytes) of the given buffer element type
RENDERER_API auto GetElementSize(eElementType etype) -> uint32_t;

/// Returns the number of single components in a given buffer element type
RENDERER_API auto GetElementCount(eElementType etype) -> uint32_t;

/// Available modes in which a VBO can be used
enum class eBufferUsage {
    STATIC,  //< A chunk of GPU memory that won't change during execution
    DYNAMIC  //< A chunk of GPU memory that could change during execution
};

/// Returns the string representation of the given buffer usage
RENDERER_API auto ToString(eBufferUsage usage) -> std::string;

/// Available format for the type of data stored in general textures
enum class eTextureFormat {
    RGB,
    RGBA,
    BGRA,
    DEPTH,
    STENCIL,
};

/// Returns the string representation of the given texture format
RENDERER_API auto ToString(eTextureFormat format) -> std::string;

/// Available storage options for a buffer of memory (how it's represented)
enum class eStorageType {
    UINT_8,
    UINT_32,
    FLOAT_32,
};

/// Returns the string representation of a given eStorageType
RENDERER_API auto ToString(eStorageType dtype) -> std::string;

/// Available wrapping modes for a texture
enum class eTextureWrap {
    REPEAT,
    REPEAT_MIRROR,
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER,
};

/// Returns the string representation of the given texture wrapping mode
RENDERER_API auto ToString(eTextureWrap tex_wrap) -> std::string;

/// Available texture filter options for a texture
enum class eTextureFilter {
    NEAREST,
    LINEAR,
    NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR,
};

/// Returns a string representation of the given texture filter option
RENDERER_API auto ToString(eTextureFilter tex_filter) -> std::string;

/// Available internal formats types for a texture
enum class eTextureIntFormat {
    RED,
    RG,
    RGB,
    RGBA,
    DEPTH,
    DEPTH_STENCIL,
};

/// Returns the string representation of the given internal format type
RENDERER_API auto ToString(eTextureIntFormat tex_iformat) -> std::string;

}  // namespace renderer
