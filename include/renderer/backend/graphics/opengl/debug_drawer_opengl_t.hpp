#pragma once

#include <vector>
#include <string>

#include <renderer/common.hpp>
#include <renderer/engine/camera_t.hpp>
#include <renderer/backend/graphics/opengl/program_opengl_t.hpp>
#include <renderer/backend/graphics/opengl/vertex_array_opengl_t.hpp>

namespace renderer {
namespace opengl {

/// Number of lines that can be drawn per batch
static constexpr uint32_t LINES_BATCH_SIZE = 1024;
/// Number of vertex positions stored per line
static constexpr uint32_t POSITIONS_PER_LINE = 2;
/// Number of vertex colors stored per line
static constexpr uint32_t COLORS_PER_LINE = 2;
/// Number of floats per vertex in each line
static constexpr uint32_t FLOATS_PER_LINE =
    3 * (POSITIONS_PER_LINE + COLORS_PER_LINE);
/// The size of the VBO (in bytes) used to store both positions and colors
static constexpr uint32_t LINES_VBO_SIZE =
    sizeof(Vec3) * (POSITIONS_PER_LINE + COLORS_PER_LINE) * LINES_BATCH_SIZE;

class RENDERER_API OpenGLDebugDrawer {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(OpenGLDebugDrawer)

    DEFINE_SMART_POINTERS(OpenGLDebugDrawer)
 public:
    struct Line {
        Vec3 start;
        Vec3 end;
        Vec3 color;

        explicit Line(Vec3 p_start, Vec3 p_end, Vec3 p_color)
            : start(p_start), end(p_end), color(p_color) {}
    };

    OpenGLDebugDrawer();

    ~OpenGLDebugDrawer() = default;

    /// Requests a line to be drawn with the given properties
    /// \param[in] start The start point of the line segment to be drawn
    /// \param[in] end The end point of the line segment to be drawn
    /// \param[in] color The color of the line to be drawn
    auto DrawLine(Vec3 start, Vec3 end, Vec3 color) -> void;

    /// Requests a cuboid to be drawn with the given properties
    /// \param[in] size The size of the box (width, depth, height)
    /// \param[in] pose The pose of the box in world space
    /// \param[in] color The color of the box to be drawn
    auto DrawBox(Vec3 size, Pose pose, Vec3 color) -> void;

    /// Renders all primitives from the given viewpoint
    /// \param[in] camera The camera used to render from
    auto Render(const Camera& camera) -> void;

    /// Resets all running counters. Call after the user reads the data
    auto ClearCounters() -> void;

    /// Returns the number of drawcalls spent during the render process
    RENDERER_NODISCARD auto num_drawcalls() const -> size_t {
        return m_NumDrawCalls;
    }

    /// Returns the number of lines drawn during the render process
    RENDERER_NODISCARD auto num_lines() const -> size_t {
        return m_NumLinesDrawn;
    }

    /// Returns a string representation of this debug drawer
    RENDERER_NODISCARD auto ToString() const -> std::string;

 protected:
    /// Renders a single batch of lines into the current render target
    /// \param[in] lines_number The number of lines to draw in this batch
    /// \param[in] lines_data A pointer to the vertex data of this batch
    auto _RenderBatchOfLines(size_t lines_number,
                             const float32_t* lines_data) -> void;

 protected:
    /// Owned reference to the main shader used for debug drawing lines
    OpenGLProgram::uptr m_LinesProgram{nullptr};

    /// VAO used to handle all lines drawing
    OpenGLVertexArray::uptr m_LinesVAO{nullptr};

    /// Container for all lines being requested by the user
    std::vector<Line> m_LinesContainer;

    /// Counter for the number of lines drawn in one step
    size_t m_NumLinesDrawn{0};

    /// Counter for the number of draw calls spent by this object
    size_t m_NumDrawCalls{0};
};

}  // namespace opengl
}  // namespace renderer
