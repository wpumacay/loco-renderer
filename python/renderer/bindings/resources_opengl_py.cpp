#include <pybind11/pybind11.h>

#include <renderer/backend/graphics/opengl/resources_manager_t.hpp>

namespace py = pybind11;

namespace renderer {
namespace opengl {

// NOLINTNEXTLINE
auto bindings_resources_manager(py::module m) -> void {
    {
        using Class = ::renderer::opengl::ResourcesManager;
        constexpr auto* ClassName = "ResourcesManager";  // NOLINT
        py::class_<Class, Class::ptr>(m, ClassName)      // NOLINT
            .def(py::init<>())
            .def("LoadProgram", &Class::LoadProgram)
            .def("LoadTexture", &Class::LoadTexture)
            .def("GetProgram", &Class::GetProgram)
            .def("GetTexture", &Class::GetTexture)
            .def("__repr__", &Class::ToString);
    }
}

}  // namespace opengl
}  // namespace renderer
