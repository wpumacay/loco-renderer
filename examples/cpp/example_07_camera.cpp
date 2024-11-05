#include <array>
#include <filesystem>
#include <memory>
#include <string>

#include <glad/gl.h>

#include <renderer/engine/graphics/window_t.hpp>
#include <renderer/backend/graphics/opengl/resources_manager_t.hpp>
#include <renderer/backend/graphics/opengl/vertex_buffer_opengl_t.hpp>
#include <renderer/backend/graphics/opengl/vertex_array_opengl_t.hpp>
#include <renderer/engine/camera_t.hpp>

#if defined(RENDERER_IMGUI)
#include <imgui.h>
#endif  // RENDERER_IMGUI

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

auto main() -> int {
    auto window = ::renderer::Window::CreateWindow(
        WINDOW_WIDTH, WINDOW_HEIGHT, ::renderer::eWindowBackend::TYPE_GLFW);
    window->RegisterKeyboardCallback([&](int key, int action, int) {
        if (key == ::renderer::keys::KEY_ESCAPE &&
            action == ::renderer::key_action::PRESSED) {
            window->RequestClose();
        }
    });

    auto resources_manager =
        std::make_shared<::renderer::opengl::ResourcesManager>();

    const auto RESOURCES_FOLDER =
        std::filesystem::path(__FILE__).parent_path().parent_path() /
        "resources";
    const auto VERT_SHADER_FILE =
        RESOURCES_FOLDER / "shaders" / "basic3d_vert.glsl";
    const auto FRAG_SHADER_FILE =
        RESOURCES_FOLDER / "shaders" / "basic3d_frag.glsl";
    auto program = resources_manager->LoadProgram(
        "basic3d", VERT_SHADER_FILE.string(), FRAG_SHADER_FILE.string());
    program->Build();

    const auto TEXTURE_FILE = RESOURCES_FOLDER / "images" / "container.jpg";
    auto texture =
        resources_manager->LoadTexture("container", TEXTURE_FILE.string());

    // clang-format off
    std::array<float, 5 * 36> buffer_data = {
        /*|    pos     |    texture   |*/
        -0.5F, -0.5F, -0.5F,  0.0F, 0.0F,
         0.5F, -0.5F, -0.5F,  1.0F, 0.0F,
         0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
         0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
        -0.5F,  0.5F, -0.5F,  0.0F, 1.0F,
        -0.5F, -0.5F, -0.5F,  0.0F, 0.0F,

        -0.5F, -0.5F,  0.5F,  0.0F, 0.0F,
         0.5F, -0.5F,  0.5F,  1.0F, 0.0F,
         0.5F,  0.5F,  0.5F,  1.0F, 1.0F,
         0.5F,  0.5F,  0.5F,  1.0F, 1.0F,
        -0.5F,  0.5F,  0.5F,  0.0F, 1.0F,
        -0.5F, -0.5F,  0.5F,  0.0F, 0.0F,

        -0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
        -0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
        -0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
        -0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
        -0.5F, -0.5F,  0.5F,  0.0F, 0.0F,
        -0.5F,  0.5F,  0.5F,  1.0F, 0.0F,

         0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
         0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
         0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
         0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
         0.5F, -0.5F,  0.5F,  0.0F, 0.0F,
         0.5F,  0.5F,  0.5F,  1.0F, 0.0F,

        -0.5F, -0.5F, -0.5F,  0.0F, 1.0F,
         0.5F, -0.5F, -0.5F,  1.0F, 1.0F,
         0.5F, -0.5F,  0.5F,  1.0F, 0.0F,
         0.5F, -0.5F,  0.5F,  1.0F, 0.0F,
        -0.5F, -0.5F,  0.5F,  0.0F, 0.0F,
        -0.5F, -0.5F, -0.5F,  0.0F, 1.0F,

        -0.5F,  0.5F, -0.5F,  0.0F, 1.0F,
         0.5F,  0.5F, -0.5F,  1.0F, 1.0F,
         0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
         0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
        -0.5F,  0.5F,  0.5F,  0.0F, 0.0F,
        -0.5F,  0.5F, -0.5F,  0.0F, 1.0F
    };
    // clang-format on

    ::renderer::opengl::OpenGLBufferLayout layout = {
        {"position", ::renderer::eElementType::FLOAT_3, false},
        {"texcoord", ::renderer::eElementType::FLOAT_2, false}};

    auto vbo = std::make_shared<::renderer::opengl::OpenGLVertexBuffer>(
        layout, renderer::eBufferUsage::STATIC,
        static_cast<uint32_t>(sizeof(buffer_data)), buffer_data.data());

    auto vao = std::make_shared<::renderer::opengl::OpenGLVertexArray>();
    vao->AddVertexBuffer(vbo);

    auto camera = std::make_shared<::renderer::Camera>("main_camera");
    camera->pose.position = {0.0F, -3.0F, 0.0F};
    camera->target = {0.0F, 0.0F, 0.0F};
    camera->data.projection = ::renderer::eProjectionType::PERSPECTIVE;

    while (window->active()) {
        window->Begin();
        program->Bind();
        texture->Bind();
        vao->Bind();

        camera->LookAt(Vec3(0.0F, 0.0F, 0.0F));

        program->SetMat4("u_model_matrix", Mat4::Identity());
        program->SetMat4("u_view_matrix", camera->ComputeViewMatrix());
        program->SetMat4("u_proj_matrix", camera->ComputeProjectionMatrix());

#if defined(RENDERER_IMGUI)
        ImGui::Begin("Options");
        {
            ImGui::SliderFloat3("camera.position", camera->pose.position.data(),
                                -10.0F, 10.0F);
        }
#endif  // RENDERER_IMGUI

        glDrawArrays(GL_TRIANGLES, 0, 36);

        vao->Unbind();
        texture->Unbind();
        program->Unbind();
        window->End();
    }
}
