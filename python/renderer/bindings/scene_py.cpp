#include <pybind11/pybind11.h>

#include <renderer/engine/scene_t.hpp>

namespace py = pybind11;

namespace renderer {

// NOLINTNEXTLINE
auto bindings_scene(py::module m) -> void {
    {
        using Class = ::renderer::Scene;
        using ParentClass = ::renderer::Object3D;
        constexpr auto* ClassName = "Scene";                      // NOLINT
        py::class_<Class, ParentClass, Class::ptr>(m, ClassName)  // NOLINT
            .def(py::init<>())
            .def("AddChild", &Class::AddChild)
            .def("ExistsChild", &Class::ExistsChild)
            .def("RemoveChild", &Class::RemoveChild)
            .def("GetChild", &Class::GetChild)
            .def("__getitem__",
                 [](Class& self, const char* name) { return self[name]; })
            .def("__repr__", &Class::ToString);
    }
}

}  // namespace renderer
