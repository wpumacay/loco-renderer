#include <array>
#include <memory>
#include <string>
#include <utility>

#include <glad/gl.h>

#include <spdlog/fmt/bundled/format.h>

#include <renderer/backend/graphics/opengl/debug_drawer_opengl_t.hpp>

namespace renderer {
namespace opengl {

constexpr const char* DD_VERT_SHADER_WIREFRAME_MODE_SRC = R"(
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 u_proj_matrix;
uniform mat4 u_view_matrix;

out vec3 f_color;

void main() {
    gl_Position = u_proj_matrix * u_view_matrix * vec4(position, 1.0);
    f_color = color;
}
)";

constexpr const char* DD_FRAG_SHADER_WIREFRAME_MODE_SRC = R"(
#version 330 core

in vec3 f_color;
out vec4 color;

void main() {
    color = vec4(f_color, 1.0);
}
)";

OpenGLDebugDrawer::OpenGLDebugDrawer() {
    m_LinesProgram = std::make_unique<OpenGLProgram>(
        DD_VERT_SHADER_WIREFRAME_MODE_SRC, DD_FRAG_SHADER_WIREFRAME_MODE_SRC);
    m_LinesProgram->Build();

    OpenGLBufferLayout buff_layout = {
        {"position", eElementType::FLOAT_3, false},
        {"color", eElementType::FLOAT_3, false}};

    auto lines_vbo = std::make_unique<OpenGLVertexBuffer>(
        buff_layout, eBufferUsage::DYNAMIC, LINES_VBO_SIZE, nullptr);

    m_LinesVAO = std::make_unique<OpenGLVertexArray>();
    m_LinesVAO->AddVertexBuffer(std::move(lines_vbo));
}

auto OpenGLDebugDrawer::DrawLine(Vec3 start, Vec3 end, Vec3 color) -> void {
    m_LinesContainer.emplace_back(start, end, color);
}

auto OpenGLDebugDrawer::Render(const Camera& camera) -> void {
    // Render all lines first --------------------------------------------------
    if (!m_LinesContainer.empty()) {
        m_LinesProgram->Bind();
        m_LinesProgram->SetMat4("u_proj_matrix",
                                camera.ComputeProjectionMatrix());
        m_LinesProgram->SetMat4("u_view_matrix", camera.ComputeViewMatrix());

        const auto NUM_FULL_BATCHES =
            m_LinesContainer.size() / LINES_BATCH_SIZE;
        const auto NUM_REMAINING_LINES =
            m_LinesContainer.size() % LINES_BATCH_SIZE;

        std::array<float, LINES_BATCH_SIZE> lines_batch{};
        for (uint32_t batch_idx = 0; batch_idx < NUM_FULL_BATCHES;
             ++batch_idx) {
            for (uint32_t line_idx = 0; line_idx < LINES_BATCH_SIZE;
                 ++line_idx) {
                const auto& line =
                    m_LinesContainer[batch_idx * LINES_BATCH_SIZE + line_idx];
                const auto VERTEX_START_IDX = FLOATS_PER_LINE * line_idx;

                // First vertex has position and color attributes
                lines_batch.at(VERTEX_START_IDX + 0) = line.start.x();
                lines_batch.at(VERTEX_START_IDX + 1) = line.start.y();
                lines_batch.at(VERTEX_START_IDX + 2) = line.start.z();

                lines_batch.at(VERTEX_START_IDX + 3) = line.color.x();
                lines_batch.at(VERTEX_START_IDX + 4) = line.color.y();
                lines_batch.at(VERTEX_START_IDX + 5) = line.color.z();

                // Second vertex has also position and color attributes
                lines_batch.at(VERTEX_START_IDX + 6) = line.end.x();
                lines_batch.at(VERTEX_START_IDX + 7) = line.end.y();
                lines_batch.at(VERTEX_START_IDX + 8) = line.end.z();

                lines_batch.at(VERTEX_START_IDX + 9) = line.color.x();
                lines_batch.at(VERTEX_START_IDX + 10) = line.color.y();
                lines_batch.at(VERTEX_START_IDX + 11) = line.color.z();
            }
            _RenderBatchOfLines(LINES_BATCH_SIZE, lines_batch.data());
        }

        if (NUM_REMAINING_LINES > 0) {
            for (uint32_t line_idx = 0; line_idx < NUM_REMAINING_LINES;
                 ++line_idx) {
                const auto& line =
                    m_LinesContainer[NUM_FULL_BATCHES * LINES_BATCH_SIZE +
                                     line_idx];
                const auto VERTEX_START_IDX = FLOATS_PER_LINE * line_idx;

                // First vertex has position and color attributes
                lines_batch.at(VERTEX_START_IDX + 0) = line.start.x();
                lines_batch.at(VERTEX_START_IDX + 1) = line.start.y();
                lines_batch.at(VERTEX_START_IDX + 2) = line.start.z();

                lines_batch.at(VERTEX_START_IDX + 3) = line.color.x();
                lines_batch.at(VERTEX_START_IDX + 4) = line.color.y();
                lines_batch.at(VERTEX_START_IDX + 5) = line.color.z();

                // Second vertex has also position and color attributes
                lines_batch.at(VERTEX_START_IDX + 6) = line.end.x();
                lines_batch.at(VERTEX_START_IDX + 7) = line.end.y();
                lines_batch.at(VERTEX_START_IDX + 8) = line.end.z();

                lines_batch.at(VERTEX_START_IDX + 9) = line.color.x();
                lines_batch.at(VERTEX_START_IDX + 10) = line.color.y();
                lines_batch.at(VERTEX_START_IDX + 11) = line.color.z();
            }
            _RenderBatchOfLines(NUM_REMAINING_LINES, lines_batch.data());
        }

        m_LinesProgram->Unbind();
        m_LinesContainer.clear();
    }
    // -------------------------------------------------------------------------

    // TODO(wilbert): render other debug primitives
}

auto OpenGLDebugDrawer::_RenderBatchOfLines(
    size_t lines_number, const float32_t* lines_data) -> void {
    auto& lines_vbo = m_LinesVAO->GetVertexBuffer(0);
    const auto VBO_SUBSIZE = (POSITIONS_PER_LINE + COLORS_PER_LINE) *
                             m_LinesContainer.size() * sizeof(Vec3);

    m_LinesVAO->Bind();
    lines_vbo.UpdateData(static_cast<uint32_t>(VBO_SUBSIZE), lines_data);
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(lines_number * 2));
    m_NumDrawCalls++;
    m_LinesVAO->Unbind();
}

auto OpenGLDebugDrawer::ClearCounters() -> void { m_NumDrawCalls = 0; }

auto OpenGLDebugDrawer::ToString() const -> std::string {
    return fmt::format(
        "<OpenGLDebugDrawer\n"
        "  linesBatchSize: {0}\n"
        "  linesCount: {1}\n"
        ">\n",
        LINES_BATCH_SIZE, m_LinesContainer.size());
}

}  // namespace opengl

}  // namespace renderer
