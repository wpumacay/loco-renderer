#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>

#include <renderer/engine/graphics/enums.hpp>
#include <renderer/engine/graphics/window_t.hpp>

#include <conversions_py.hpp>
#include <math/common.hpp>

namespace py = pybind11;

namespace renderer {

// NOLINTNEXTLINE
void bindings_window(py::module m) {
    {
        using Class = renderer::WindowConfig;
        constexpr auto ClassName = "WindowConfig";  // NOLINT
        py::class_<Class>(m, ClassName)
            .def(py::init<>())
            .def_readwrite("backend", &Class::backend)
            .def_readwrite("width", &Class::width)
            .def_readwrite("height", &Class::height)
            .def_readwrite("title", &Class::title)
            .def_readwrite("clear_color", &Class::clear_color)
            .def_readwrite("gl_version_major", &Class::gl_version_major)
            .def_readwrite("gl_version_minor", &Class::gl_version_minor)
            .def("__repr__", [](const Class& self) -> py::str {
                return py::str(
                           "<WindowConfig\n"
                           "  backend: {}\n"
                           "  width: {}\n"
                           "  height: {}\n"
                           "  title: \"{}\"\n"
                           "  clear_color: {}\n"
                           "  gl_version_major: {}\n"
                           "  gl_version_minor: {}\n"
                           ">")
                    .format(ToString(self.backend), self.width, self.height,
                            self.title, self.clear_color.toString(),
                            self.gl_version_major, self.gl_version_minor);
            });
    }

    {
        using Class = renderer::Window;
        constexpr auto ClassName = "Window";  // NOLINT
        py::class_<Class, Class::ptr>(m, ClassName)
            .def_static("Create",
                        static_cast<Class::ptr (*)(int, int, eWindowBackend)>(
                            ::renderer::Window::Create))
            .def_static("Create", static_cast<Class::ptr (*)(WindowConfig)>(
                                      ::renderer::Window::Create))
            .def("EnableCursor", &Class::EnableCursor)
            .def("DisableCursor", &Class::DisableCursor)
            .def("Begin", &Class::Begin)
            .def("End", &Class::End)
            .def("RequestClose", &Class::RequestClose)
            .def("RegisterKeyboardCallback", &Class::RegisterKeyboardCallback)
            .def("RegisterMouseButtonCallback",
                 &Class::RegisterMouseButtonCallback)
            .def("RegisterMouseMoveCallback", &Class::RegisterMouseMoveCallback)
            .def("RegisterScrollCallback", &Class::RegisterScrollCallback)
            .def("RegisterResizeCallback", &Class::RegisterResizeCallback)
            .def("SetClearColor",
                 [](Class& self,
                    const py::array_t<::math::float32_t>& array_np) -> void {
                     self.SetClearColor(
                         ::math::nparray_to_vec4<::math::float32_t>(array_np));
                 })
            .def_property_readonly("active", &Class::active)
            .def_property_readonly("clear_color", &Class::clear_color)
            .def_property_readonly("width", &Class::width)
            .def_property_readonly("height", &Class::height)
            .def_property_readonly("title", &Class::title)
            .def_property_readonly("backend", &Class::backend)
            .def("__repr__", [](const Class& self) -> py::str {
                return py::str(
                           "<Window\n"
                           "  title: \"{}\"\n"
                           "  active: {}\n"
                           "  width: {}\n"
                           "  height: {}\n"
                           "  clear_color: {}\n"
                           "  backend: {}\n"
                           ">")
                    .format(self.title(), self.active(), self.width(),
                            self.height(), self.clear_color().toString(),
                            ToString(self.backend()));
            });
    }
}

}  // namespace renderer
