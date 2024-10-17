#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>

#include <utils/logging.hpp>

namespace py = pybind11;

namespace renderer {

extern auto bindings_enums(py::module m) -> void;
extern auto bindings_window(py::module m) -> void;
extern auto bindings_keycodes(py::module m) -> void;
extern auto bindings_buttons(py::module m) -> void;
extern auto bindings_program(py::module m) -> void;
extern auto bindings_buffers(py::module m) -> void;

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
    ::renderer::bindings_program(m);
    ::renderer::bindings_buffers(m);
}
