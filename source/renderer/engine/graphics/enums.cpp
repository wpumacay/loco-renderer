#include <string>

#include <renderer/engine/graphics/enums.hpp>

namespace renderer {

auto ToString(eGraphicsAPI api) -> std::string {
    switch (api) {
        case eGraphicsAPI::OPENGL:
            return "opengl";
        case eGraphicsAPI::VULKAN:
            return "vulkan";
        case eGraphicsAPI::DIRECTX11:
            return "directx11";
        case eGraphicsAPI::DIRECTX12:
            return "directx12";
        default:
            return "none";
    }
}

auto ToString(eWindowBackend type) -> std::string {
    switch (type) {
        case eWindowBackend::TYPE_NONE:
            return "none";
        case eWindowBackend::TYPE_GLFW:
            return "glfw";
        case eWindowBackend::TYPE_EGL:
            return "egl";
        default:
            return "none";
    }
}

auto ToString(eShaderType type) -> std::string {
    switch (type) {
        case eShaderType::VERTEX:
            return "vertex";
        case eShaderType::FRAGMENT:
            return "fragment";
        case eShaderType::GEOMETRY:
            return "geometry";
        case eShaderType::COMPUTE:
            return "compute";
        default:
            return "vertex";
    }
}

auto ToString(eElementType etype) -> std::string {
    switch (etype) {
        case eElementType::FLOAT_1:
            return "Float1";
        case eElementType::FLOAT_2:
            return "Float2";
        case eElementType::FLOAT_3:
            return "Float3";
        case eElementType::FLOAT_4:
            return "Float4";
        case eElementType::INT_1:
            return "Int1";
        case eElementType::INT_2:
            return "Int2";
        case eElementType::INT_3:
            return "Int3";
        case eElementType::INT_4:
            return "Int4";
        default:
            return "undefined";
    }
}

auto GetElementSize(eElementType etype) -> uint32_t {
    switch (etype) {
        case eElementType::FLOAT_1:
            return 4 * 1;
        case eElementType::FLOAT_2:
            return 4 * 2;
        case eElementType::FLOAT_3:
            return 4 * 3;
        case eElementType::FLOAT_4:
            return 4 * 4;
        case eElementType::INT_1:
            return 4 * 1;
        case eElementType::INT_2:
            return 4 * 2;
        case eElementType::INT_3:
            return 4 * 3;
        case eElementType::INT_4:
            return 4 * 4;
        default:
            return 4;
    }
}

auto GetElementCount(eElementType etype) -> uint32_t {
    switch (etype) {
        case eElementType::FLOAT_1:
            return 1;
        case eElementType::FLOAT_2:
            return 2;
        case eElementType::FLOAT_3:
            return 3;
        case eElementType::FLOAT_4:
            return 4;
        case eElementType::INT_1:
            return 1;
        case eElementType::INT_2:
            return 2;
        case eElementType::INT_3:
            return 3;
        case eElementType::INT_4:
            return 4;
        default:
            return 1;
    }
}

auto ToString(eBufferUsage usage) -> std::string {
    switch (usage) {
        case eBufferUsage::STATIC:
            return "Static";
        case eBufferUsage::DYNAMIC:
            return "Dynamic";
        default:
            return "undefined";
    }
}

auto ToString(eTextureFormat format) -> std::string {
    switch (format) {
        case eTextureFormat::RGB:
            return "rgb";
        case eTextureFormat::RGBA:
            return "rgba";
        case eTextureFormat::BGRA:
            return "bgra";
        case eTextureFormat::DEPTH:
            return "depth";
        case eTextureFormat::STENCIL:
            return "stencil";
        default:
            return "undefined";
    }
}

auto ToString(eStorageType dtype) -> std::string {
    switch (dtype) {
        case eStorageType::UINT_8:
            return "uint_8";
        case eStorageType::UINT_32:
            return "uint_32";
        case eStorageType::FLOAT_32:
            return "float_32";
        default:
            return "undefined";
    }
}

}  // namespace renderer
