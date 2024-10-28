#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <renderer/engine/object_t.hpp>

PYBIND11_MAKE_OPAQUE(std::vector<::renderer::Object3D>);

namespace py = pybind11;

namespace renderer {

// NOLINTNEXTLINE
auto bindings_object3d(py::module m) -> void {
    {
        using Enum = ::renderer::eObjectType;
        py::enum_<Enum>(m, "ObjectType")
            .value("BASE", Enum::BASE)
            .value("MESH", Enum::MESH)
            .value("CAMERA", Enum::CAMERA)
            .value("LIGHT", Enum::LIGHT);
    }

    {
        using Class = ::renderer::Object3D;
        constexpr auto* ClassName = "Object3D";      // NOLINT
        py::class_<Class, Class::ptr>(m, ClassName)  // NOLINT
            .def(py::init<>())
            .def("AddChild", &Class::AddChild)
            .def_readwrite("pose", &Class::pose)
            .def_property(
                "parent",
                [](Class& self) -> Class::ptr {
                    if (auto holder = self.parent.lock()) {
                        return holder;
                    }
                    return nullptr;
                },
                // NOLINTNEXTLINE
                [](Class& self, Class::ptr parent_obj) {
                    self.parent = parent_obj;
                })
            .def_readwrite("children", &Class::children)
            .def_property_readonly("type", &Class::type)
            .def("__repr__", &Class::ToString);
    }
}

}  // namespace renderer
