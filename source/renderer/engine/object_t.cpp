#include <string>
#include <utility>

#include <spdlog/fmt/bundled/format.h>

#include <renderer/engine/object_t.hpp>

namespace renderer {

auto ToString(eObjectType type) -> std::string {
    switch (type) {
        case eObjectType::BASE:
            return "Base";
        case eObjectType::MESH:
            return "Mesh";
        default:
            return "Base";
    }
}

Object3D::Object3D(Pose init_pose) : pose(init_pose) {}

Object3D::Object3D(Vec3 init_pos) : pose(Pose(init_pos, Quat())) {}

auto Object3D::AddChild(Object3D::ptr child_obj) -> void {
    child_obj->parent = shared_from_this();
    this->children.push_back(std::move(child_obj));
}

auto Object3D::ToString() const -> std::string {
    return fmt::format(
        "<Object3D\n"
        "  position: {0}\n"
        "  orientation: {1}\n"
        "  parent: {2}\n"
        "  children: {3}\n"
        ">\n",
        this->pose.position.toString(), this->pose.orientation.toString(),
        (!this->parent.expired() ? "Yes" : "None"), this->children.size());
}

}  // namespace renderer
