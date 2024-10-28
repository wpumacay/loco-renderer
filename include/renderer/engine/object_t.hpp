#pragma once

#include <memory>
#include <string>
#include <vector>

#include <renderer/common.hpp>

namespace renderer {

/// All valid object types in the engine
enum class RENDERER_API eObjectType : uint8_t {
    BASE = 0,
    MESH = 1,
    CAMERA = 2,
    LIGHT = 3,
};

/// Returns a string representation of the given object type enum
RENDERER_API auto ToString(eObjectType type) -> std::string;

/// Base interface for all objects supported by the engine
class RENDERER_API Object3D {
    DEFAULT_COPY_AND_MOVE_AND_ASSIGN(Object3D)

    DEFINE_SMART_POINTERS(Object3D)

 public:
    /// Creates a default Object3D
    Object3D() = default;

    /// Creates an object with given pose in world space
    /// \param[in] init_pose The pose of this object in world space
    explicit Object3D(Pose init_pose);

    /// Creates an object with given position and orientation in world space
    /// \param[in] init_pos The position of this object in world space
    explicit Object3D(Vec3 init_pos);

    /// Deallocates the resources used by this object
    virtual ~Object3D() = default;

    /// Returns the type of this object
    RENDERER_NODISCARD auto type() const -> eObjectType { return m_Type; }

    /// Returns a string representation of this object
    RENDERER_NODISCARD auto ToString() const -> std::string;

 public:
    /// The 3d pose of this object respect to world space. If object has a
    /// parent object, then this pose is relative to that parent object
    Pose pose{};

    /// A weak reference to the parent of this object
    std::weak_ptr<Object3D> parent;

    /// A container for the children of this object
    std::vector<Object3D::ptr> children;

 protected:
    /// The type of this object
    eObjectType m_Type{eObjectType::BASE};
};

}  // namespace renderer
