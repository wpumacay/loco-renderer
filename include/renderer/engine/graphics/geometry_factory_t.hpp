#pragma once

#include <renderer/engine/graphics/geometry_t.hpp>

namespace renderer {

/// Axis used define the main axis when creating some geometry types
enum class eAxis {
    AXIS_X,  ///< Main axis is the X axis
    AXIS_Y,  ///< Main axis is the Y axis
    AXIS_Z   ///< Main axis is the Z axis
};

/// Creates the geometry for a plane given the width and depth
/// \param[in] width The width of the plane (x-dimension)
/// \param[in] depth The depth of the plane (y-dimension)
/// \param[in] widthSegments The number of segments along the width of the plane
/// \param[in] depthSegments The number of segments along the depth of the plane
auto CreatePlane(float width, float depth, size_t widthSegments = 1,
                 size_t depthSegments = 1) -> Geometry::uptr;

/// Creates the geometry for a box given the dimensions
/// \param[in] width The width of the box (x-dimension)
/// \param[in] depth The depth of the box (y-dimension)
/// \param[in] height The height of the box (z-dimension)
auto CreateBox(float width, float depth, float height) -> Geometry::uptr;

}  // namespace renderer
