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
    m_NumLinesDrawn++;
}

auto OpenGLDebugDrawer::DrawBox(Vec3 size, Pose pose, Vec3 color) -> void {
    const auto HALF_WIDTH = 0.5F * size.x();
    const auto HALF_DEPTH = 0.5F * size.y();
    const auto HALF_HEIGHT = 0.5F * size.z();

    constexpr size_t NUM_BOX_VERTICES = 8;

    std::array<Vec3, NUM_BOX_VERTICES> vertices = {
        Vec3(HALF_WIDTH, -HALF_DEPTH, -HALF_HEIGHT),
        Vec3(HALF_WIDTH, HALF_DEPTH, -HALF_HEIGHT),
        Vec3(-HALF_WIDTH, HALF_DEPTH, -HALF_HEIGHT),
        Vec3(-HALF_WIDTH, -HALF_DEPTH, -HALF_HEIGHT),
        Vec3(HALF_WIDTH, -HALF_DEPTH, HALF_HEIGHT),
        Vec3(HALF_WIDTH, HALF_DEPTH, HALF_HEIGHT),
        Vec3(-HALF_WIDTH, HALF_DEPTH, HALF_HEIGHT),
        Vec3(-HALF_WIDTH, -HALF_DEPTH, HALF_HEIGHT)};

    for (auto& vertex : vertices) {
        vertex = pose * vertex;
    }

    DrawLine(vertices[0], vertices[1], color);
    DrawLine(vertices[1], vertices[2], color);
    DrawLine(vertices[2], vertices[3], color);
    DrawLine(vertices[3], vertices[0], color);

    DrawLine(vertices[1], vertices[5], color);
    DrawLine(vertices[5], vertices[6], color);
    DrawLine(vertices[6], vertices[2], color);
    DrawLine(vertices[2], vertices[1], color);

    DrawLine(vertices[5], vertices[6], color);
    DrawLine(vertices[6], vertices[7], color);
    DrawLine(vertices[7], vertices[4], color);
    DrawLine(vertices[4], vertices[5], color);

    DrawLine(vertices[4], vertices[7], color);
    DrawLine(vertices[7], vertices[3], color);
    DrawLine(vertices[3], vertices[0], color);
    DrawLine(vertices[0], vertices[4], color);

    DrawLine(vertices[2], vertices[6], color);
    DrawLine(vertices[6], vertices[7], color);
    DrawLine(vertices[7], vertices[3], color);
    DrawLine(vertices[3], vertices[2], color);

    DrawLine(vertices[1], vertices[5], color);
    DrawLine(vertices[5], vertices[4], color);
    DrawLine(vertices[4], vertices[0], color);
    DrawLine(vertices[0], vertices[1], color);
}

auto OpenGLDebugDrawer::DrawSphere(float radius, Pose pose,
                                   Vec3 color) -> void {
    constexpr size_t NUM_CIRCLE_POINTS = 20;
    // Create the vertices for the main circles (xy, yz, xz)
    std::array<Vec3, NUM_CIRCLE_POINTS> circle_pts_xy;
    std::array<Vec3, NUM_CIRCLE_POINTS> circle_pts_yz;
    std::array<Vec3, NUM_CIRCLE_POINTS> circle_pts_xz;
    for (size_t i = 0; i < NUM_CIRCLE_POINTS; ++i) {
        auto theta = 2.0F * PI * static_cast<float>(i) /
                     static_cast<float>(NUM_CIRCLE_POINTS);
        auto r_cos_theta = radius * std::cos(theta);
        auto r_sin_theta = radius * std::sin(theta);
        circle_pts_xy.at(i) = {r_cos_theta, r_sin_theta, 0.0F};
        circle_pts_yz.at(i) = {0.0F, r_cos_theta, r_sin_theta};
        circle_pts_xz.at(i) = {r_sin_theta, 0.0F, r_cos_theta};
    }

    // Transform all points into world space
    for (auto& vertex : circle_pts_xy) {
        vertex = pose * vertex;
    }
    for (auto& vertex : circle_pts_yz) {
        vertex = pose * vertex;
    }
    for (auto& vertex : circle_pts_xz) {
        vertex = pose * vertex;
    }

    // Draw each main circle
    for (size_t i = 0; i < circle_pts_xy.size(); ++i) {
        DrawLine(circle_pts_xy.at(i % circle_pts_xy.size()),
                 circle_pts_xy.at((i + 1) % circle_pts_xy.size()), color);
    }
    for (size_t i = 0; i < circle_pts_yz.size(); ++i) {
        DrawLine(circle_pts_yz.at(i % circle_pts_yz.size()),
                 circle_pts_yz.at((i + 1) % circle_pts_yz.size()), color);
    }
    for (size_t i = 0; i < circle_pts_xz.size(); ++i) {
        DrawLine(circle_pts_xz.at(i % circle_pts_xz.size()),
                 circle_pts_xz.at((i + 1) % circle_pts_xz.size()), color);
    }
}

auto OpenGLDebugDrawer::DrawCylinder(float radius, float height, Pose pose,
                                     Vec3 color) -> void {
    constexpr size_t NUM_CIRCLE_POINTS = 20;
    // Create first main rectangle
    std::array<Vec3, 4> rect_pts_yz;
    rect_pts_yz.at(0) = Vec3(0.0F, -radius, -0.5F * height);
    rect_pts_yz.at(1) = Vec3(0.0F, radius, -0.5F * height);
    rect_pts_yz.at(2) = Vec3(0.0F, radius, 0.5F * height);
    rect_pts_yz.at(3) = Vec3(0.0F, -radius, 0.5F * height);

    // Create second main rectangle
    std::array<Vec3, 4> rect_pts_xz;
    rect_pts_xz.at(0) = Vec3(-radius, 0.0F, -0.5F * height);
    rect_pts_xz.at(1) = Vec3(radius, 0.0F, -0.5F * height);
    rect_pts_xz.at(2) = Vec3(radius, 0.0F, 0.5F * height);
    rect_pts_xz.at(3) = Vec3(-radius, 0.0F, 0.5F * height);

    // Create top and bottom base circles
    std::array<Vec3, NUM_CIRCLE_POINTS> circle_pts_top;
    std::array<Vec3, NUM_CIRCLE_POINTS> circle_pts_bottom;
    for (size_t i = 0; i < NUM_CIRCLE_POINTS; ++i) {
        auto theta = 2.0F * PI * static_cast<float>(i) /
                     static_cast<float>(NUM_CIRCLE_POINTS);
        auto r_cos_theta = radius * std::cos(theta);
        auto r_sin_theta = radius * std::sin(theta);
        circle_pts_top.at(i) = {r_cos_theta, r_sin_theta, 0.5F * height};
        circle_pts_bottom.at(i) = {r_cos_theta, r_sin_theta, -0.5F * height};
    }

    // Transform all points into world space
    for (auto& vertex : rect_pts_yz) {
        vertex = pose * vertex;
    }
    for (auto& vertex : rect_pts_xz) {
        vertex = pose * vertex;
    }
    for (auto& vertex : circle_pts_top) {
        vertex = pose * vertex;
    }
    for (auto& vertex : circle_pts_bottom) {
        vertex = pose * vertex;
    }

    // Draw all components of our cylinder
    for (size_t i = 0; i < rect_pts_yz.size(); ++i) {
        DrawLine(rect_pts_yz.at(i % rect_pts_yz.size()),
                 rect_pts_yz.at((i + 1) % rect_pts_yz.size()), color);
    }
    for (size_t i = 0; i < rect_pts_xz.size(); ++i) {
        DrawLine(rect_pts_xz.at(i % rect_pts_xz.size()),
                 rect_pts_xz.at((i + 1) % rect_pts_xz.size()), color);
    }
    for (size_t i = 0; i < circle_pts_top.size(); ++i) {
        DrawLine(circle_pts_top.at(i % circle_pts_top.size()),
                 circle_pts_top.at((i + 1) % circle_pts_top.size()), color);
    }
    for (size_t i = 0; i < circle_pts_bottom.size(); ++i) {
        DrawLine(circle_pts_bottom.at(i % circle_pts_bottom.size()),
                 circle_pts_bottom.at((i + 1) % circle_pts_bottom.size()),
                 color);
    }
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

        std::array<float, LINES_BATCH_SIZE * FLOATS_PER_LINE> lines_batch{};
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

auto OpenGLDebugDrawer::ClearCounters() -> void {
    m_NumDrawCalls = 0;
    m_NumLinesDrawn = 0;
}

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
