# cmake-format: off
loco_create_target(stb INTERFACE
  SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image.h
    ${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image_write.h
  INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR})

loco_create_target(glad SHARED
  SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/gl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/egl.c
  INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/glad)

target_compile_definitions(glad PRIVATE -DGLAD_API_CALL_EXPORT_BUILD)

# cmake-format: on
