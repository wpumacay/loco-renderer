#pragma once

#include <string>

#include <renderer/common.hpp>
#include <renderer/engine/camera_t.hpp>
#include <renderer/engine/scene_t.hpp>

namespace renderer {

class RENDERER_API IRenderer {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(IRenderer)

    DEFINE_SMART_POINTERS(IRenderer)
 public:
    IRenderer() = default;

    virtual ~IRenderer() = default;

    /// Draws a line segment with given start and end points
    /// \param[in] start The start position of the line segment
    /// \param[in] end The end position of the line segment
    /// \param[in] color The color of the line
    virtual auto DrawLine(Vec3 start, Vec3 end, Vec3 color) -> void = 0;

    /// Renders the given scene to the current render target
    /// \param[in] scene The scene to be rendered
    /// \param[in] camera The camera to use as viewpoint for the render
    virtual auto Render(Scene::ptr scene, Camera::ptr camera) -> void = 0;

    /// Enables/Disables the rendering pipeline (apart from debug drawer)
    auto SetEnabled(bool enable) -> void { m_Enabled = enable; }

    /// Enables/Disables the debug drawer pipeline
    auto SetDebugEnabled(bool enable) -> void { m_DebugEnabled = enable; }

    /// Returns whether or not the renderer is enabled
    RENDERER_NODISCARD auto enabled() const -> bool { return m_Enabled; }

    /// Returns whether or not the renderer's debug drawer is enabled
    RENDERER_NODISCARD auto debugEnabled() const -> bool {
        return m_DebugEnabled;
    }

    RENDERER_NODISCARD virtual auto ToString() const -> std::string;

 protected:
    /// Whether or not the renderer is enabled
    bool m_Enabled{true};

    /// Whether or not the debug drawer is enabled
    bool m_DebugEnabled{true};
};

}  // namespace renderer
