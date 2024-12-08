#include <chrono>
#include <memory>

#include <glad/gl.h>

#include <renderer/engine/graphics/window_t.hpp>
#include <renderer/backend/graphics/opengl/program_opengl_t.hpp>

#include <utils/logging.hpp>

constexpr const char* VERT_SHADER_SRC = R"(
    #version 330 core

    layout (location = 0) in vec3 pos;

    void main() {
        gl_Position = vec4(pos, 1.0f);
    }
)";

constexpr const char* FRAG_SHADER_SRC = R"(
    #version 330 core

    out vec4 output_color;

    uniform float u_time;

    void main() {
        output_color = vec4(abs(sin(u_time)), 0.0f, 0.0f, 1.0f);
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
    win_config.title = "Example 02 - OpenGL Program";
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

    if (program->IsValid()) {
        LOG_INFO("Shader Program successfully built and ready for action");
    } else {
        LOG_ERROR("Shader Program couldn't be built");
    }

    // clang-format off
    // NOLINTNEXTLINE
    float vertices[] = {
        -0.5, -0.5, 0.0,
         0.5, -0.5, 0.0,
        -0.5,  0.5, 0.0,
         0.5, -0.5, 0.0,
         0.5,  0.5, 0.0,
        -0.5,  0.5, 0.0,
    };
    // clang-format on

    // NOLINTNEXTLINE
    GLuint vao = 0, vbo = 0;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Prepare the layout for our data -----------------------------------------
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
                 static_cast<const float*>(vertices), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    // -------------------------------------------------------------------------

    auto t_start = std::chrono::steady_clock::now();
    while (window->active()) {
        window->Begin();
        auto t_now = std::chrono::steady_clock::now();
        auto t_current =
            std::chrono::duration_cast<std::chrono::duration<float>>(t_now -
                                                                     t_start)
                .count();

        program->Bind();
        program->SetFloat("u_time", t_current);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        program->Unbind();

        window->End();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    return 0;
}
