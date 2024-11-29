#include <cmath>
#include <string>

#include <spdlog/fmt/bundled/format.h>

#include <renderer/engine/camera_t.hpp>

namespace renderer {

auto CameraData::ToString() const -> std::string {
    return fmt::format(
        "<CameraData\n"
        "  projection={0}\n"
        "  fov={1}\n"
        "  aspect={2}\n"
        "  width={3}\n"
        "  height={4}\n"
        "  near={5}\n"
        "  far={6}\n"
        ">\n",
        ::renderer::ToString(projection), fov, aspect, width, height, near,
        far);
}

Camera::Camera(const char* name) : Object3D(name) {}

auto Camera::ComputeViewMatrix() const -> Mat4 {
    // The view matrix is the inverse of the camera pose in world space. We
    // could use the inverse directly, but we have the analytical inverse
    // of this matrix (to avoid extra unnecessary computations)
    //
    //  |                 |   | --- right^T ---  -<right, p>   |
    //  |    R^T   -R^T p |   | ---   up^T  ---  -<up, p>      |
    //  |                 | = | --- front^T ---  -<front, p>   |
    //  | 0   0   0    1  |   |  0     0    0          1       |

    Mat4 view_mat{};

    view_mat(0, 0) = this->v_right.x();
    view_mat(1, 0) = this->v_up.x();
    view_mat(2, 0) = this->v_front.x();
    view_mat(3, 0) = 0.0F;

    view_mat(0, 1) = this->v_right.y();
    view_mat(1, 1) = this->v_up.y();
    view_mat(2, 1) = this->v_front.y();
    view_mat(3, 1) = 0.0F;

    view_mat(0, 2) = this->v_right.z();
    view_mat(1, 2) = this->v_up.z();
    view_mat(2, 2) = this->v_front.z();
    view_mat(3, 2) = 0.0F;

    view_mat(0, 3) = -::math::dot<float>(this->v_right, this->pose.position);
    view_mat(1, 3) = -::math::dot<float>(this->v_up, this->pose.position);
    view_mat(2, 3) = -::math::dot<float>(this->v_front, this->pose.position);
    view_mat(3, 3) = 1.0F;

    return view_mat;
}

auto Camera::ComputeProjectionMatrix() const -> Mat4 {
    switch (this->data.projection) {
        case eProjectionType::PERSPECTIVE: {
            // Based on ThreeJS implementation
            auto top = this->data.near *
                       std::tan((this->data.fov * 0.5F) * PI / 180.0F) /
                       this->zoom;
            auto height = 2.0F * top;
            auto width = this->data.aspect * height;
            auto left = -0.5F * width;

            return Mat4::Perspective(left, left + width, top, top - height,
                                     this->data.near, this->data.far);
        }
        case eProjectionType::ORTHOGRAPHIC: {
            return Mat4::Ortho(this->data.width / this->zoom,
                               this->data.height / this->zoom, this->data.near,
                               this->data.far);
        }
    }
    return Mat4::Identity();
}

auto Camera::LookAt(Vec3 point) -> void {
    this->target = point;
    // Adapted the look-at function from [0]. Handles corners cases in which the
    // front vector aligns with the world-up vector (just use the world axes)

    // Cache front vector, in case the update cant be applied
    auto old_front = this->v_front;
    this->v_front =
        ::math::normalize<float>(this->pose.position - this->target);
    const bool FRONT_ALIGNS_WORLDUP =
        (this->v_front == this->worldUp) || (this->v_front == -this->worldUp);

    if (FRONT_ALIGNS_WORLDUP) {
        // Keep cached value of the front vector
        this->v_front = old_front;
        return;
    }

    this->v_right =
        ::math::normalize(math::cross<float>(this->worldUp, this->v_front));
    this->v_up =
        ::math::normalize(math::cross<float>(this->v_front, this->v_right));

    // Get the orientation from the basis vectors (rot-matrix)
    Mat3 rotmat(this->v_right, this->v_up, v_front);
    this->pose.orientation = Quat(rotmat);
}

auto Camera::LookAt(Quat orientation) -> void {
    auto rot_matrix = Mat3(orientation);
    this->v_right = ::math::normalize(rot_matrix[0]);
    this->v_up = ::math::normalize(rot_matrix[1]);
    this->v_front = ::math::normalize(rot_matrix[2]);
    // Recompute the target point, making sure we are at the same distance we
    // were previous to this update
    auto length = ::math::norm(this->pose.position - this->target);
    this->target =
        this->pose.position - static_cast<double>(length) * this->v_front;
}

auto Camera::ToString() const -> std::string {
    return fmt::format(
        "<Camera\n"
        "  name={0}\n"
        "  position={1}\n"
        "  orientation={2}\n"
        "  target={3}\n"
        "  zoom={4}\n"
        "  front={5}\n"
        "  right={6}\n"
        "  up={7}\n"
        ">\n",
        m_Name, this->pose.position.toString(),
        this->pose.orientation.toString(), this->target.toString(), this->zoom,
        this->v_front.toString(), this->v_right.toString(),
        this->v_up.toString());
}

}  // namespace renderer
