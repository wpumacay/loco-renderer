if(NOT TARGET RendererCpp)
  return()
endif()

# cmake-format: off
set(RENDERER_EXAMPLES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/example_01_window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/example_02_shaders_opengl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/example_03_buffers_opengl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/example_04_buffers_with_indices_opengl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/example_05_textures_opengl.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/example_06_managers.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/example_07_camera.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/example_08_geometry.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/example_09_geometry_factory.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/example_10_light_casters.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/example_11_camera_controllers.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/example_12_debug_drawing.cpp
)
# cmake-format: on

foreach(example_filepath IN LISTS RENDERER_EXAMPLES_LIST)
  # cmake-format: off
  loco_setup_single_file_example(
      ${example_filepath}
      TARGET_DEPENDENCIES RendererCpp
      INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})
  # cmake-format: on
endforeach()
