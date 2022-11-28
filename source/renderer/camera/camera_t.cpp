#include <renderer/camera/camera_t.hpp>

namespace renderer {

auto ToString(const eProjectionType& proj_type) -> std::string {
    switch (proj_type) {
        case eProjectionType::PERSPECTIVE:
            return "perspective";
        case eProjectionType::ORTHOGRAPHIC:
            return "orthographic";
    }
}

Camera::Camera(const Vec3& position, const Vec3& target, const Vec3& world_up,
               const ProjectionData& proj_data)
    : m_Position(position),
      m_Target(target),
      m_WorldUp(world_up),
      m_ProjData(proj_data) {
    _ComputeBasisVectors();
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

auto Camera::_ComputeBasisVectors() -> void {
    // Adapted the look-at function from [0]. Handles corners cases in which the
    // front vector aligns with the world-up vector (just use the world axes)
    m_Front = math::normalize<float>(m_Position - m_Target);
    if (m_Front == m_WorldUp) {
        // In case of aligning with the world-up vector, use the standard world
        // axes for the required basis vectors
        m_Front = m_WorldUp;
        m_Right = Vec3(m_WorldUp.z(), m_WorldUp.x(), m_WorldUp.y());
        m_Up = math::cross<float>(m_Front, m_Right);
    } else if (m_Front == -m_WorldUp) {
        m_Front = -m_WorldUp;
        m_Right = Vec3(m_WorldUp.z(), m_WorldUp.x(), m_WorldUp.y());
        m_Up = math::cross<float>(m_Front, m_Right);
    } else {
        m_Right = math::cross<float>(m_WorldUp, m_Front);
        m_Up = math::cross<float>(m_Front, m_Right);
    }
    // Get the orientation from the basis vectors (rot-matrix)
    Mat3 rotmat(m_Right, m_Up, m_Front);
    m_Orientation = Quat::FromRotationMatrix(rotmat);
}

auto Camera::UpdateViewMatrix() -> void {
    // The view matrix is the inverse of the camera pose in world space. We
    // could use the inverse directly, but we have the analytical inverse
    // of this matrix (to avoid extra unnecessary computations)
    //
    //  |                 |   | --- right^T ---  -<right, p>   |
    //  |    R^T   -R^T p |   | ---   up^T  ---  -<up, p>      |
    //  |                 | = | --- front^T ---  -<front, p>   |
    //  | 0   0   0    1  |   |  0     0    0          1       |

    m_ViewMatrix(0, 0) = m_Right.x();
    m_ViewMatrix(1, 0) = m_Up.x();
    m_ViewMatrix(2, 0) = m_Front.x();
    m_ViewMatrix(3, 0) = 0.0F;

    m_ViewMatrix(0, 1) = m_Right.y();
    m_ViewMatrix(1, 1) = m_Up.y();
    m_ViewMatrix(2, 1) = m_Front.y();
    m_ViewMatrix(3, 1) = 0.0F;

    m_ViewMatrix(0, 2) = m_Right.z();
    m_ViewMatrix(1, 2) = m_Up.z();
    m_ViewMatrix(2, 2) = m_Front.z();
    m_ViewMatrix(3, 2) = 0.0F;

    m_ViewMatrix(0, 3) = -math::dot<float>(m_Right, m_Position);
    m_ViewMatrix(1, 3) = -math::dot<float>(m_Up, m_Position);
    m_ViewMatrix(2, 3) = -math::dot<float>(m_Front, m_Position);
    m_ViewMatrix(3, 3) = 1.0F;
}

auto Camera::UpdateProjectionMatrix() -> void {
    switch (m_ProjData.projection) {
        case eProjectionType::PERSPECTIVE: {
            m_ProjMatrix = Mat4::Perspective(m_ProjData.fov, m_ProjData.aspect,
                                             m_ProjData.near, m_ProjData.far);
            break;
        }
        case eProjectionType::ORTHOGRAPHIC: {
            m_ProjMatrix = Mat4::Ortho(m_ProjData.width, m_ProjData.height,
                                       m_ProjData.near, m_ProjData.far);
        }
    }
}

auto Camera::SetPosition(const Vec3& pos) -> void {
    m_Position = pos;
    _ComputeBasisVectors();
    UpdateViewMatrix();
}

auto Camera::SetTarget(const Vec3& target) -> void {
    m_Target = target;
    _ComputeBasisVectors();
    UpdateViewMatrix();
}

auto Camera::SetProjectionData(const ProjectionData& proj_data) -> void {
    m_ProjData = proj_data;
    UpdateProjectionMatrix();
}

auto Camera::SetFOV(float fov) -> void {
    if (m_ProjData.projection == eProjectionType::PERSPECTIVE) {
        m_ProjData.fov = fov;
        UpdateProjectionMatrix();
    }
}

auto Camera::SetAspectRatio(float aspect) -> void {
    if (m_ProjData.projection == eProjectionType::PERSPECTIVE) {
        m_ProjData.aspect = aspect;
        UpdateProjectionMatrix();
    }
}

auto Camera::SetWidth(float width) -> void {
    if (m_ProjData.projection == eProjectionType::ORTHOGRAPHIC) {
        m_ProjData.width = width;
        UpdateProjectionMatrix();
    }
}

auto Camera::SetHeight(float height) -> void {
    if (m_ProjData.projection == eProjectionType::ORTHOGRAPHIC) {
        m_ProjData.height = height;
        UpdateProjectionMatrix();
    }
}

auto Camera::SetZNear(float near) -> void {
    m_ProjData.near = near;
    UpdateProjectionMatrix();
}

auto Camera::SetZFar(float far) -> void {
    m_ProjData.far = far;
    UpdateProjectionMatrix();
}

}  // namespace renderer