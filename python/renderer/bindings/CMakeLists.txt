if(NOT TARGET renderer::renderer)
  return()
endif()

# cmake-format: off
set(BINDINGS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/enums_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/window_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/keycodes_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/buttons_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/program_opengl_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/buffers_opengl_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/texture_opengl_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/resources_opengl_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/input_manager_py.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/camera_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/object_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene_py.cpp
)
# cmake-format: on

pybind11_add_module(renderer_bindings MODULE ${BINDINGS_SOURCES})
target_link_libraries(renderer_bindings PRIVATE renderer::renderer)
target_include_directories(renderer_bindings PRIVATE)
