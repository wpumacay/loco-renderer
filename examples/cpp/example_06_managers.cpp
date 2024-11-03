#include <chrono>
#include <filesystem>
#include <memory>
#include <string>

#include <glad/gl.h>

#include <renderer/engine/graphics/window_t.hpp>
#include <renderer/backend/graphics/opengl/resources_manager_t.hpp>
#include <renderer/engine/input_manager_t.hpp>
#include <renderer/backend/graphics/opengl/vertex_buffer_opengl_t.hpp>
#include <renderer/backend/graphics/opengl/index_buffer_opengl_t.hpp>
#include <renderer/backend/graphics/opengl/vertex_array_opengl_t.hpp>

#include <utils/logging.hpp>

#if defined(RENDERER_IMGUI)
#include <imgui.h>
#endif  // RENDERER_IMGUI

struct Engine {
    ::renderer::Window::ptr window{nullptr};
    ::renderer::opengl::ResourcesManager::ptr resources_manager{nullptr};
    ::renderer::InputManager::ptr input_manager{nullptr};
    std::chrono::steady_clock::time_point stamp_now{};
    std::chrono::steady_clock::time_point stamp_bef{};
    double time = 0.0;
    double delta = 0.0;
};

// NOLINTNEXTLINE
Engine g_engine;

auto main() -> int {
    ::renderer::WindowConfig win_config;
    win_config.backend = ::renderer::eWindowBackend::TYPE_GLFW;
    win_config.width = 1024;
    win_config.height = 768;
    win_config.title = "Example 06 - Managers";
    win_config.gl_version_major = 3;
    win_config.gl_version_minor = 3;

    g_engine.window = ::renderer::Window::CreateWindow(win_config);
    g_engine.window->RegisterKeyboardCallback([&](int key, int action, int) {
        g_engine.input_manager->CallbackKey(key, action);
        if (key == ::renderer::keys::KEY_ESCAPE) {
            g_engine.window->RequestClose();
        }
    });
    g_engine.window->RegisterMouseButtonCallback(
        [&](int button, int action, double x, double y) {
            g_engine.input_manager->CallbackMouseButton(
                button, action, static_cast<float>(x), static_cast<float>(y));
        });
    g_engine.window->RegisterMouseMoveCallback([&](double x, double y) {
        g_engine.input_manager->CallbackMouseMove(static_cast<float>(x),
                                                  static_cast<float>(y));
    });
    g_engine.window->RegisterScrollCallback([&](double xOff, double yOff) {
        g_engine.input_manager->CallbackScroll(static_cast<float>(xOff),
                                               static_cast<float>(yOff));
    });

    g_engine.resources_manager =
        std::make_shared<::renderer::opengl::ResourcesManager>();
    g_engine.input_manager = std::make_shared<::renderer::InputManager>();

    const auto SHADERS_FOLDER =
        std::filesystem::path(__FILE__).parent_path().parent_path() /
        "resources" / "shaders";
    const auto VERT_SHADER_FILE = SHADERS_FOLDER / "basic2d_vert.glsl";
    const auto FRAG_SHADER_FILE = SHADERS_FOLDER / "basic2d_frag.glsl";
    auto program = g_engine.resources_manager->LoadProgram(
        "basic2d", VERT_SHADER_FILE.string(), FRAG_SHADER_FILE.string());
    program->Build();
    if (!program->IsValid()) {
        LOG_CORE_ERROR("There was an error building the shader program");
        return 1;
    }

    const auto IMAGES_FOLDER =
        std::filesystem::path(__FILE__).parent_path().parent_path() /
        "resources" / "images";
    const auto TEXTURE_FILE = IMAGES_FOLDER / "container.jpg";
    auto texture = g_engine.resources_manager->LoadTexture(
        "container", TEXTURE_FILE.string());

    // -------------------------------------------------------------------------
    // Setup the primitive to be drawn

    // clang-format off
    // NOLINTNEXTLINE
    float buffer_data[] = {
        /*|   pos   |  texture  |*/
        -0.5F, -0.5F, 0.0F, 0.0F, // NOLINT
         0.5F, -0.5F, 2.0F, 0.0F, // NOLINT
         0.5F,  0.5F, 2.0F, 2.0F, // NOLINT
        -0.5F,  0.5F, 0.0F, 2.0F // NOLINT
    };

    // NOLINTNEXTLINE
    uint32_t buffer_indices[] = {
        0, 1, 2,  // first triangle
        0, 2, 3  // second triangle
    };

    constexpr uint32_t NUM_VERTICES = 6;
    // clang-format on

    ::renderer::opengl::OpenGLBufferLayout layout = {
        {"position", ::renderer::eElementType::FLOAT_2, false},
        {"texcoord", ::renderer::eElementType::FLOAT_2, false}};

    auto vbo = std::make_shared<::renderer::opengl::OpenGLVertexBuffer>(
        layout, renderer::eBufferUsage::STATIC,
        static_cast<uint32_t>(sizeof(buffer_data)), buffer_data);
    auto ibo = std::make_shared<::renderer::opengl::OpenGLIndexBuffer>(
        renderer::eBufferUsage::STATIC, NUM_VERTICES, buffer_indices);

    auto vao = std::make_shared<::renderer::opengl::OpenGLVertexArray>();
    vao->AddVertexBuffer(vbo);
    vao->SetIndexBuffer(ibo);
    // -------------------------------------------------------------------------

    g_engine.stamp_bef = std::chrono::steady_clock::now();
    g_engine.stamp_now = std::chrono::steady_clock::now();

    while (g_engine.window->active()) {
        g_engine.window->Begin();

        if (g_engine.input_manager->IsKeyDown(::renderer::keys::KEY_ESCAPE)) {
            g_engine.window->RequestClose();
        }

        auto cursor_position = g_engine.input_manager->GetCursorPosition();
        LOG_INFO("Cursorposition: ({0},{1})", cursor_position.x(),
                 cursor_position.y());
        LOG_INFO("MouseButtonLeft: {0}", g_engine.input_manager->IsMouseDown(
                                             renderer::mouse::BUTTON_LEFT));
        LOG_INFO("MouseButtonMiddle: {0}", g_engine.input_manager->IsMouseDown(
                                               renderer::mouse::BUTTON_MIDDLE));
        LOG_INFO("MouseButtonRight: {0}", g_engine.input_manager->IsMouseDown(
                                              renderer::mouse::BUTTON_RIGHT));

        // Draw the cube
        {
            program->Bind();
            texture->Bind();
            vao->Bind();

            glDrawElements(GL_TRIANGLES, NUM_VERTICES, GL_UNSIGNED_INT,
                           nullptr);

            vao->Unbind();
            texture->Unbind();
            program->Unbind();
        }
#if defined(RENDERER_IMGUI)
        ImGui::Begin("Demo options");
        {
            // NOLINTNEXTLINE
            ImGui::Text("time: %.5f, delta: %.5f", g_engine.time,
                        g_engine.delta);
        }
#endif  // RENDERER_IMGUI

        g_engine.window->End();

        // Keep track of time
        {
            g_engine.stamp_now = std::chrono::steady_clock::now();
            g_engine.delta =
                std::chrono::duration_cast<std::chrono::duration<double>>(
                    g_engine.stamp_now - g_engine.stamp_bef)
                    .count();
            g_engine.time += g_engine.delta;
            g_engine.stamp_bef = g_engine.stamp_now;
        }
    }

    return 0;
}
