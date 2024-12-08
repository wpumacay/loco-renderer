#include <array>
#include <memory>
#include <utility>

#include <glad/gl.h>

#include <utils/logging.hpp>

#include <renderer/engine/graphics/window_t.hpp>
#include <renderer/backend/graphics/opengl/program_opengl_t.hpp>
#include <renderer/backend/graphics/opengl/vertex_buffer_opengl_t.hpp>
#include <renderer/backend/graphics/opengl/index_buffer_opengl_t.hpp>
#include <renderer/backend/graphics/opengl/vertex_array_opengl_t.hpp>

constexpr const char* VERT_SHADER_SRC = R"(
    #version 330 core

    layout (location = 0) in vec2 position;
    layout (location = 1) in vec3 color;

    out vec3 frag_color;

    void main() {
        gl_Position = vec4(position, 0.0f, 1.0f);
        frag_color = color;
    }
)";

constexpr const char* FRAG_SHADER_SRC = R"(
    #version 330 core

    in vec3 frag_color;
    out vec4 output_color;

    void main() {
        output_color = vec4(frag_color, 1.0f);
    }
)";

auto main() -> int {
    constexpr int WINDOW_WIDTH = 1024;
    constexpr int WINDOW_HEIGHT = 768;
    constexpr auto WINDOW_API = ::renderer::eWindowBackend::TYPE_GLFW;

    renderer::WindowConfig win_config;
    win_config.backend = WINDOW_API;
    win_config.width = WINDOW_WIDTH;
    win_config.height = WINDOW_HEIGHT;
    win_config.title = "Example 04 - OpenGL Index Buffers";
    win_config.gl_version_major = 3;
    win_config.gl_version_minor = 3;

    auto window = ::renderer::Window::Create(win_config);
    window->RegisterKeyboardCallback([&](int key, int, int) {
        if (key == ::renderer::keys::KEY_ESCAPE) {
            window->RequestClose();
        }
    });

    auto program = std::make_shared<::renderer::opengl::OpenGLProgram>(
        VERT_SHADER_SRC, FRAG_SHADER_SRC);
    program->Build();

    if (!program->IsValid()) {
        LOG_CORE_ERROR("There was an error building the shader program");
        return 1;
    }

    // clang-format off
    // NOLINTNEXTLINE
    float buffer_data[] = {
    /*|      pos     |     color      |*/
        -0.5F, -0.5F, 1.0F, 0.0F, 0.0F, // NOLINT
         0.5F, -0.5F, 0.0F, 1.0F, 0.0F, // NOLINT
         0.5F,  0.5F, 0.0F, 0.0F, 1.0F, // NOLINT
        -0.5F,  0.5F, 1.0F, 1.0F, 1.0F // NOLINT
    };
    // clang-format on
    constexpr uint32_t NUM_INDICES = 6;

    // NOLINTNEXTLINE
    uint32_t buffer_indices[] = {0, 1, 2, 0, 2, 3};

    ::renderer::opengl::OpenGLBufferLayout layout = {
        {"position", ::renderer::eElementType::FLOAT_2, false},
        {"color", ::renderer::eElementType::FLOAT_3, false}};

    auto vbo = std::make_unique<::renderer::opengl::OpenGLVertexBuffer>(
        layout, ::renderer::eBufferUsage::STATIC, sizeof(buffer_data),
        buffer_data);

    auto ibo = std::make_unique<::renderer::opengl::OpenGLIndexBuffer>(
        renderer::eBufferUsage::STATIC, NUM_INDICES, buffer_indices);

    auto vao = std::make_unique<::renderer::opengl::OpenGLVertexArray>();
    vao->AddVertexBuffer(std::move(vbo));
    vao->SetIndexBuffer(std::move(ibo));

    while (window->active()) {
        window->Begin();
        program->Bind();
        vao->Bind();

        glDrawElements(GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_INT, nullptr);

        vao->Unbind();
        program->Unbind();
        window->End();
    }

    return 0;
}
