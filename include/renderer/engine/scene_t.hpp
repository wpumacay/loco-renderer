#pragma once

#include <string>
#include <unordered_map>

#include <renderer/common.hpp>
#include <renderer/engine/object_t.hpp>

namespace renderer {

/// Scene container for engine objects of various types
class RENDERER_API Scene : public Object3D {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Scene)

    DEFINE_SMART_POINTERS(Scene)

 public:
    /// Creates an empty scene
    Scene();

    /// Deallocates any owned resources by this scene
    ~Scene() override = default;

    /// Adds the given object to the list of this scene's children
    auto AddChild(Object3D::ptr obj) -> void override;

    /// Returns whether or not an object with given name exists in the scene
    auto ExistsChild(const std::string& name) -> bool;

    /// Removes the object with given name
    auto RemoveChild(const std::string& name) -> void;

    /// Returns the object requested by name
    RENDERER_NODISCARD auto GetChild(const std::string& name) -> Object3D::ptr;

    /// Returns a reference to the object requested by name
    auto operator[](const char* name) -> Object3D::ptr;

    /// Returns a string representation of this scene
    RENDERER_NODISCARD auto ToString() const -> std::string override;

 private:
    /// Map used for storing object names and link to index in storage
    std::unordered_map<std::string, size_t> m_Name2Id;
};

}  // namespace renderer
