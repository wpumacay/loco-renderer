#pragma once

#include <string>

#include <renderer/engine/graphics/enums.hpp>
#include <renderer/engine/object_t.hpp>

namespace renderer {

/// Parameters required to define the projection appropriately
struct RENDERER_API CameraData {
    /// Type of projection used by the camera
    eProjectionType projection{eProjectionType::PERSPECTIVE};
    /// Field of view of the camera (for perspective cameras only)
    float fov{45.0F};
    /// Aspect ratio of the view-frustum (for perspective cameras only)
    float aspect{1.0F};
    /// Width of view box of the camera (for orthographic cameras only)
    float width{2.0F};
    /// Height of the view box of the camera (for orthographics cameras only)
    float height{2.0F};
    /// Distance to the closest plane of the view-volume
    float near{0.1F};
    /// Distance to the furthest plane of the view-volume
    float far{100.0F};

    /// Returns the string representation of this object
    RENDERER_NODISCARD auto ToString() const -> std::string;
};

class RENDERER_API Camera : public Object3D {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Camera)

    DEFINE_SMART_POINTERS(Camera)

 public:
    explicit Camera(const char* name);

    ~Camera() override = default;

    /// Computes the view matrix from the current state of the camera
    auto ComputeViewMatrix() const -> Mat4;

    /// Computes the projection matrix from the current state of the camera
    auto ComputeProjectionMatrix() const -> Mat4;

    /// Computes the basis vectors from the given target point
    auto LookAt(Vec3 point) -> void;

    /// Computes the basis vectors from the given target orientation
    auto LookAt(Quat orientation) -> void;

    /// Returns the string representation of this camera
    RENDERER_NODISCARD auto ToString() const -> std::string override;

 public:
    /// Projection data for this camera's configuration
    CameraData data{};

    /// Target point of the camera w.r.t. the world frame
    Vec3 target;

    /// Value of the zoom used for this camera
    float zoom{1.0F};

    /// Z-axis of the reference frame of this camera
    Vec3 v_front;

    /// Y-axis of the reference frame of this camera
    Vec3 v_up;

    /// X-axis of the reference frame of this camera
    Vec3 v_right;

    /// World-up vector used by this camera setting
    Vec3 worldUp = {0.0, 0.0, 1.0};
};

}  // namespace renderer
