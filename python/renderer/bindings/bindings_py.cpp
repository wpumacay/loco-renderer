#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>

#include <utils/logging.hpp>

namespace py = pybind11;

namespace renderer {

extern auto bindings_enums(py::module m) -> void;
extern auto bindings_window(py::module m) -> void;
extern auto bindings_keycodes(py::module m) -> void;
extern auto bindings_buttons(py::module m) -> void;
extern auto bindings_input_manager(py::module m) -> void;

namespace opengl {
extern auto bindings_program(py::module m) -> void;
extern auto bindings_buffers(py::module m) -> void;
extern auto bindings_texture(py::module m) -> void;
extern auto bindings_resources_manager(py::module m) -> void;
}  // namespace opengl

extern auto bindings_object3d(py::module m) -> void;
extern auto bindings_scene(py::module m) -> void;

}  // namespace renderer

// NOLINTNEXTLINE
PYBIND11_MODULE(renderer_bindings, m) {
    try {
        py::module::import("math3d");
    } catch (py::error_already_set& e) {
        e.restore();
        LOG_CORE_ERROR(
            "Didn't find required module math3d. Won't be able to access "
            "fields that are math3d types, it will likely crash :(");
    }

    ::renderer::bindings_enums(m);
    ::renderer::bindings_window(m);
    ::renderer::bindings_keycodes(m);
    ::renderer::bindings_buttons(m);
    ::renderer::bindings_input_manager(m);

    auto m_opengl = m.def_submodule("opengl");
    ::renderer::opengl::bindings_program(m_opengl);
    ::renderer::opengl::bindings_buffers(m_opengl);
    ::renderer::opengl::bindings_texture(m_opengl);
    ::renderer::opengl::bindings_resources_manager(m_opengl);

    ::renderer::bindings_object3d(m);
    ::renderer::bindings_scene(m);
}
