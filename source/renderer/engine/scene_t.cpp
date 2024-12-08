#include <string>
#include <utility>

#include <utils/logging.hpp>

#include <spdlog/fmt/bundled/format.h>

#include <renderer/engine/scene_t.hpp>

namespace renderer {

Scene::Scene() : Object3D("scene") {}

auto Scene::AddChild(Object3D::ptr obj) -> void {
    if (m_Name2Id.find(obj->name()) != m_Name2Id.end()) {
        LOG_CORE_WARN(
            "Scene::AddChild >>> object \"{0}\" already exists in the scene. "
            "Won't add to avoid duplicates",
            obj->name());
        return;
    }

    auto obj_name = obj->name();
    obj->parent = shared_from_this();
    this->children.push_back(std::move(obj));
    m_Name2Id[obj_name] = this->children.size() - 1;
}

auto Scene::ExistsChild(const std::string& name) -> bool {
    return m_Name2Id.find(name) != m_Name2Id.end();
}

auto Scene::RemoveChild(const std::string& name) -> void {
    if (!ExistsChild(name)) {
        LOG_CORE_WARN(
            "Scene::RemoveChild >>> object with name \"{0}\" doesn't "
            "exists in scene. Won't remove anything for the moment",
            name);
        return;
    }

    auto obj_idx = m_Name2Id.at(name);
    this->children.erase(this->children.begin() + obj_idx);
    m_Name2Id.erase(name);
    for (auto& [obj_name, idx] : m_Name2Id) {
        if (idx > obj_idx) {
            idx--;
        }
    }
}

auto Scene::GetChild(const std::string& name) -> Object3D::ptr {
    if (!ExistsChild(name)) {
        return nullptr;
    }

    auto obj_idx = m_Name2Id.at(name);
    if (obj_idx < 0 || obj_idx >= this->children.size()) {
        LOG_CORE_WARN(
            "Scene::GetChild >>> object {0} has index out of range {1} "
            "/ {2}",
            name, obj_idx, this->children.size());
        return nullptr;
    }

    return this->children[static_cast<size_t>(obj_idx)];
}

auto Scene::operator[](const char* name) -> Object3D::ptr {
    return GetChild(name);
}

auto Scene::ToString() const -> std::string {
    return fmt::format(
        "<Scene\n"
        "  children: {0}\n"
        "?\n",
        this->children.size());
}

}  // namespace renderer
