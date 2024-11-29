#pragma once

#include <memory>
#include <string>
#include <vector>

#include <renderer/common.hpp>
#include <renderer/engine/graphics/enums.hpp>

namespace renderer {

/// Base interface for all objects supported by the engine
class RENDERER_API Object3D : public std::enable_shared_from_this<Object3D> {
    DEFAULT_COPY_AND_MOVE_AND_ASSIGN(Object3D)

    DEFINE_SMART_POINTERS(Object3D)

 public:
    /// Creates a default Object3D
    explicit Object3D(const char* name);

    /// Creates an object with given pose in world space
    /// \param[in] name The name of this object
    /// \param[in] init_pose The pose of this object in world space
    explicit Object3D(const char* name, Pose init_pose);

    /// Creates an object with given position in world space
    /// \param[in] name The name of this object
    /// \param[in] init_pos The position of this object in world space
    explicit Object3D(const char* name, Vec3 init_pos);

    /// Deallocates the resources used by this object
    virtual ~Object3D() = default;

    /// Adds the given object as child of this object
    virtual auto AddChild(Object3D::ptr child_obj) -> void;

    /// Returns the type of this object
    RENDERER_NODISCARD auto type() const -> eObjectType { return m_Type; }

    /// Returns a copy of the name of the object
    RENDERER_NODISCARD auto name() const -> std::string { return m_Name; }

    /// Returns a string representation of this object
    RENDERER_NODISCARD virtual auto ToString() const -> std::string;

 public:
    /// The 3d pose of this object respect to world space. If object has a
    /// parent object, then this pose is relative to that parent object
    Pose pose;

    /// A weak reference to the parent of this object
    std::weak_ptr<Object3D> parent;

    /// A container for the children of this object
    std::vector<Object3D::ptr> children;

 protected:
    /// The type of this object
    eObjectType m_Type{eObjectType::BASE};

    /// A unique identifier of this object
    std::string m_Name;
};

}  // namespace renderer
