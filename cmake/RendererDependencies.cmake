include_guard()

# ~~~
# CMake configuration for third-party dependencies.
#
# Dependencies:
# * catch2
# * glfw
# * imgui
# * utils
# * math
# * pybind11
#
# - Based on the superbuild script by jeffamstutz for ospray
#   https://github.com/jeffamstutz/superbuild_ospray/blob/main/macros.cmake
# - Based on MuJoCo's helper find_or_fetch content macro
#   https://github.com/deepmind/mujoco/blob/main/cmake/FindOrFetch.cmake
# ~~~

# -------------------------------------
set(RENDERER_DEP_VERSION_catch2
    182c910b4b63ff587a3440e08f84f70497e49a81 # Release 2.13.10
    CACHE STRING "Version of Catch2 to be fetched (used for unittests)")

set(RENDERER_DEP_VERSION_glfw
    7482de6071d21db77a7236155da44c172a7f6c9e # Release 3.3.8
    CACHE STRING
          "Version of GLFW to be fetched (used to create an xplatf. window) ")

# set(RENDERER_DEP_VERSION_imgui c6e0284ac58b3f205c95365478888f7b53b077e2 #
# Release 1.89.9 CACHE STRING "Version of Dear-ImGui to be fetched (used for
# prototyping UI)")

set(RENDERER_DEP_VERSION_utils
    a84f6afa35d71eb5e5b97f16476e013386e3d244 # Version v0.2.12
    CACHE STRING "Version of internal utilities repo to be fetched")

set(RENDERER_DEP_VERSION_math
    d0a29407901b35f39b9cf94b1e4dfea604e6c2a7 # Version v0.6.11
    CACHE STRING "Version of internal math repo to be fetched")

set(RENDERER_DEP_VERSION_pybind11
    8a099e44b3d5f85b20f05828d919d2332a8de841 # Release v2.11.1
    CACHE STRING "Version of PyBind11 to be fetched (used for python bindings)")

mark_as_advanced(RENDERER_DEP_VERSION_catch2)
mark_as_advanced(RENDERER_DEP_VERSION_glfw)
# mark_as_advanced(RENDERER_DEP_VERSION_imgui)
mark_as_advanced(RENDERER_DEP_VERSION_utils)
mark_as_advanced(RENDERER_DEP_VERSION_math)
mark_as_advanced(RENDERER_DEP_VERSION_pybind11)

# -------------------------------------
find_package(OpenGL REQUIRED)

if(OpenGL_EGL_FOUND)
  target_link_libraries(OpenGL::OpenGL INTERFACE OpenGL::EGL)
endif()

# -------------------------------------
option(FIND_OR_FETCH_USE_SYSTEM_PACKAGE
       "Whether or not to give priority to system-wide package search" OFF)

# cmake-format: off
# ------------------------------------------------------------------------------
# Pybind11 is used for generating Python bindings for this project's C++ API.
# ------------------------------------------------------------------------------

set(PYBIND11_TEST OFF CACHE BOOL "" FORCE)

loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE ${FIND_OR_FETCH_USE_SYSTEM_PACKAGE}
  PACKAGE_NAME pybind11
  LIBRARY_NAME pybind11
  GIT_REPO https://github.com/pybind/pybind11.git
  GIT_TAG ${RENDERER_DEP_VERSION_pybind11}
  GIT_PROGRESS FALSE
  GIT_SHALLOW TRUE
  TARGETS pybind11::headers
  EXCLUDE_FROM_ALL)

# ------------------------------------------------------------------------------
# Catch2 is used for making unit-tests in C++ land. It's API is simple yet quite
# powerfull (e.g. we can make use of template-parametrized tests-cases)
# ------------------------------------------------------------------------------

set(CATCH_INSTALL_DOCS OFF CACHE BOOL "" FORCE)
set(CATCH_INSTALL_EXTRAS OFF CACHE BOOL "" FORCE)
set(CATCH_DEVELOPMENT_BUILD OFF CACHE BOOL "" FORCE)

loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE ${FIND_OR_FETCH_USE_SYSTEM_PACKAGE}
  PACKAGE_NAME Catch2
  LIBRARY_NAME catch2
  GIT_REPO https://github.com/catchorg/Catch2.git
  GIT_TAG ${RENDERER_DEP_VERSION_catch2}
  GIT_PROGRESS FALSE
  GIT_SHALLOW TRUE
  TARGETS Catch2::Catch2
  EXCLUDE_FROM_ALL)

# Add custom scripts for test-case registration to the module path
if (catch2_POPULATED)
  list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/contrib")
endif()

# ------------------------------------------------------------------------------
# Use GLFW from sources (as it might not be installed in the system). We use
# GLFW in order to create a window with a proper GL context in many platforms.
# Recall that this might not be the only backend for window creation (e.g. Qt)
# ------------------------------------------------------------------------------

set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE ${FIND_OR_FETCH_USE_SYSTEM_PACKAGE}
  PACKAGE_NAME glfw3
  LIBRARY_NAME glfw3
  GIT_REPO https://github.com/glfw/glfw.git
  GIT_TAG ${RENDERER_DEP_VERSION_glfw}
  GIT_PROGRESS FALSE
  GIT_SHALLOW TRUE
  TARGETS glfw
  EXCLUDE_FROM_ALL)
# Make an alias (sorry, kind of an OCD thingy xD)
add_library(glfw::glfw ALIAS glfw)

# ------------------------------------------------------------------------------
# ImGui is used for fast prototyping Graphical User Interfaces in Immediate mode
# ------------------------------------------------------------------------------

#### FetchContent_Declare(
####   imgui
####   GIT_REPOSITORY https://github.com/ocornut/imgui.git
####   GIT_TAG ${RENDERER_DEP_VERSION_imgui}
####   GIT_PROGRESS FALSE
####   GIT_SHALLOW FALSE
####   USES_TERMINAL_DOWNLOAD TRUE
####   PATCH_COMMAND
####     "${GIT_EXECUTABLE}"
####     "apply"
####     "${CMAKE_CURRENT_SOURCE_DIR}/cmake/imgui-gcc-warns.patch"
####     "||"
####     "${CMAKE_COMMAND}"
####     "-E"
####     "true")
#### FetchContent_GetProperties(imgui)
####
#### if (NOT imgui_POPULATED)
####   FetchContent_Populate(imgui)
#### endif()

# ------------------------------------------------------------------------------
# Math3d is used as math library (defines vectors, matrices, and operations
# that could be used on these types). The API is similar to Eigen's
# ------------------------------------------------------------------------------

set(MATH_BUILD_PYTHON_BINDINGS OFF CACHE BOOL "" FORCE)
set(MATH_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(MATH_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(MATH_BUILD_DOCS OFF CACHE BOOL "" FORCE)

loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  LIBRARY_NAME math
  GIT_REPO https://github.com/wpumacay/math.git
  GIT_TAG ${RENDERER_DEP_VERSION_math}
  GIT_PROGRESS FALSE
  GIT_SHALLOW FALSE
  TARGETS math::math
  EXCLUDE_FROM_ALL)

# ------------------------------------------------------------------------------
# "Utils" exposes some utilities that we'll use (like logs, profiling, etc.)
# ------------------------------------------------------------------------------

set(UTILS_BUILD_PYTHON_BINDINGS OFF CACHE BOOL "" FORCE)
set(UTILS_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(UTILS_BUILD_DOCS OFF CACHE BOOL "" FORCE)

loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  LIBRARY_NAME utils
  GIT_REPO https://github.com/wpumacay/utils.git
  GIT_TAG ${RENDERER_DEP_VERSION_utils}
  GIT_PROGRESS FALSE
  GIT_SHALLOW FALSE
  TARGETS utils::utils
  EXCLUDE_FROM_ALL)

# cmake-format: on
