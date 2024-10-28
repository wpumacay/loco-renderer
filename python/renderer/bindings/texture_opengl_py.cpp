#include <memory>
#include <stdexcept>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <utils/logging.hpp>

#include <renderer/backend/graphics/opengl/texture_data_opengl_t.hpp>
#include <renderer/backend/graphics/opengl/texture_opengl_t.hpp>

namespace py = pybind11;

namespace renderer {
namespace opengl {

// NOLINTNEXTLINE
auto bindings_texture(py::module m) -> void {
    {
        using Class = ::renderer::opengl::OpenGLTextureData;
        constexpr auto* ClassName = "OpenGLTextureData";  // NOLINT
        py::class_<Class, Class::ptr>(m, ClassName)
            .def(py::init<const char*>())
            .def(py::init([](const py::array_t<uint8_t>& np_data)
                              -> Class::ptr {
                auto info = np_data.request();
                if (info.ndim < 3) {
                    throw std::runtime_error(
                        "OpenGLTextureData > numpy constructor only supports "
                        "RGB and RGBA images for now");
                }

                auto width = info.shape[1];
                auto height = info.shape[0];
                auto channels = info.shape[2];

                return std::make_shared<::renderer::opengl::OpenGLTextureData>(
                    width, height, channels,
                    static_cast<const uint8_t*>(info.ptr));
            }))
            .def_property_readonly("width", &Class::width)
            .def_property_readonly("height", &Class::height)
            .def_property_readonly("channels", &Class::channels)
            .def_property_readonly("image_path", &Class::image_path)
            .def_property_readonly("format", &Class::format)
            .def_property_readonly("storage", &Class::storage)
            .def("numpy",
                 [](Class& self) -> py::array_t<uint8_t> {
                     auto width = self.width();
                     auto height = self.height();
                     auto depth = self.channels();

                     return py::array_t<uint8_t>({height, width, depth},
                                                 {width * depth, depth, 1},
                                                 self.data(), py::cast(self));
                 })
            .def("__repr__", &Class::ToString);
    }

    {
        using Class = ::renderer::opengl::OpenGLTexture;
        constexpr auto* ClassName = "OpenGLTexture";  // NOLINT
        py::class_<Class, Class::ptr>(m, ClassName)
            .def(py::init<const char*>())
            .def(py::init([](OpenGLTextureData::ptr tex_data) -> Class::ptr {
                return std::make_shared<Class>(std::move(tex_data));
            }))
            .def("Bind", &Class::Bind)
            .def("Unbind", &Class::Unbind)
            .def_property("border_color", &Class::border_color,
                          &Class::SetBorderColor)
            .def_property("min_filter", &Class::min_filter,
                          &Class::SetMinFilter)
            .def_property("mag_filter", &Class::mag_filter,
                          &Class::SetMagFilter)
            .def_property("wrap_mode_u", &Class::wrap_mode_u,
                          &Class::SetWrapModeU)
            .def_property("wrap_mode_v", &Class::wrap_mode_v,
                          &Class::SetWrapModeV)
            .def_property_readonly("internal_format", &Class::internal_format)
            .def_property_readonly("opengl_id", &Class::opengl_id)
            .def("texture_data", &Class::texture_data)
            .def("__repr__", &Class::ToString);
    }
}

}  // namespace opengl

}  // namespace renderer
