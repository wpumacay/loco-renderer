#include <pybind11/pybind11.h>

#include <renderer/engine/graphics/enums.hpp>

namespace py = pybind11;

namespace renderer {

// NOLINTNEXTLINE
auto bindings_enums(py::module m) -> void {
    {
        using Enum = ::renderer::eGraphicsAPI;
        py::enum_<Enum>(m, "GraphicsAPI")
            .value("OPENGL", Enum::OPENGL)
            .value("VULKAN", Enum::VULKAN)
            .value("DIRECTX11", Enum::DIRECTX11)
            .value("DIRECTX12", Enum::DIRECTX12);
    }

    {
        using Enum = ::renderer::eWindowBackend;
        py::enum_<Enum>(m, "WindowBackend")
            .value("TYPE_NONE", Enum::TYPE_NONE)
            .value("TYPE_GLFW", Enum::TYPE_GLFW)
            .value("TYPE_EGL", Enum::TYPE_EGL);
    }

    {
        using Enum = ::renderer::eShaderType;
        py::enum_<Enum>(m, "ShaderType")
            .value("VERTEX", Enum::VERTEX)
            .value("FRAGMENT", Enum::FRAGMENT)
            .value("GEOMETRY", Enum::GEOMETRY)
            .value("COMPUTE", Enum::COMPUTE);
    }

    {
        using Enum = ::renderer::eElementType;
        py::enum_<Enum>(m, "ElementType")
            .value("FLOAT_1", Enum::FLOAT_1)
            .value("FLOAT_2", Enum::FLOAT_2)
            .value("FLOAT_3", Enum::FLOAT_3)
            .value("FLOAT_4", Enum::FLOAT_4)
            .value("INT_1", Enum::INT_1)
            .value("INT_2", Enum::INT_2)
            .value("INT_3", Enum::INT_3)
            .value("INT_4", Enum::INT_4);
    }

    {
        using Enum = ::renderer::eBufferUsage;
        py::enum_<Enum>(m, "BufferUsage")
            .value("STATIC", Enum::STATIC)
            .value("DYNAMIC", Enum::DYNAMIC);
    }

    {
        using Enum = ::renderer::eTextureFormat;
        py::enum_<Enum>(m, "TextureFormat")
            .value("RGB", Enum::RGB)
            .value("RGBA", Enum::RGBA)
            .value("BGRA", Enum::BGRA)
            .value("DEPTH", Enum::DEPTH)
            .value("STENCIL", Enum::STENCIL);
    }

    {
        using Enum = ::renderer::eStorageType;
        py::enum_<Enum>(m, "StorageType")
            .value("UINT_8", Enum::UINT_8)
            .value("UINT_32", Enum::UINT_32)
            .value("FLOAT_32", Enum::FLOAT_32);
    }

    {
        using Enum = ::renderer::eTextureWrap;
        py::enum_<Enum>(m, "TextureWrap")
            .value("REPEAT", Enum::REPEAT)
            .value("REPEAT_MIRROR", Enum::REPEAT_MIRROR)
            .value("CLAMP_TO_EDGE", Enum::CLAMP_TO_EDGE)
            .value("CLAMP_TO_BORDER", Enum::CLAMP_TO_BORDER);
    }

    {
        using Enum = ::renderer::eTextureFilter;
        py::enum_<Enum>(m, "TextureFilter")
            .value("NEAREST", Enum::NEAREST)
            .value("LINEAR", Enum::LINEAR)
            .value("NEAREST_MIPMAP_NEAREST", Enum::NEAREST_MIPMAP_NEAREST)
            .value("LINEAR_MIPMAP_NEAREST", Enum::LINEAR_MIPMAP_NEAREST)
            .value("NEAREST_MIPMAP_LINEAR", Enum::NEAREST_MIPMAP_LINEAR)
            .value("LINEAR_MIPMAP_LINEAR", Enum::LINEAR_MIPMAP_LINEAR);
    }

    {
        using Enum = ::renderer::eTextureIntFormat;
        py::enum_<Enum>(m, "TextureIntFormat")
            .value("RED", Enum::RED)
            .value("RG", Enum::RG)
            .value("RGB", Enum::RGB)
            .value("RGBA", Enum::RGBA)
            .value("DEPTH", Enum::DEPTH)
            .value("DEPTH_STENCIL", Enum::DEPTH_STENCIL);
    }
}

}  // namespace renderer
