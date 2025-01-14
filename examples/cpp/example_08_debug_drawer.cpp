#include <memory>

#include <glad/gl.h>

#include <utils/timing.hpp>

#include <renderer/engine/camera_t.hpp>
#include <renderer/engine/orbit_camera_controller_t.hpp>
#include <renderer/engine/graphics/window_t.hpp>
#include <renderer/backend/graphics/opengl/debug_drawer_opengl_t.hpp>

#if defined(RENDERER_IMGUI)
#include <imgui.h>
#endif  // RENDERER_IMGUI

auto main() -> int {
    ::utils::Clock::Init();

    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 600;

    auto window = ::renderer::Window::Create(
        WINDOW_WIDTH, WINDOW_HEIGHT, ::renderer::eWindowBackend::TYPE_GLFW);
    window->RegisterKeyboardCallback([&](int key, int action, int) {
        if (key == ::renderer::keys::KEY_ESCAPE &&
            action == ::renderer::key_action::PRESSED) {
            window->RequestClose();
        }
    });

    auto debug_drawer =
        std::make_unique<::renderer::opengl::OpenGLDebugDrawer>();

    constexpr float FRUSTUM_SIZE = 20.0F;
    auto camera = std::make_shared<::renderer::Camera>("main_camera");
    camera->pose.position = {0.0F, -3.0F, 0.0F};
    camera->target = {0.0F, 0.0F, 0.0F};
    camera->data.projection = ::renderer::eProjectionType::PERSPECTIVE;

    auto camera_controller =
        std::make_shared<::renderer::OrbitCameraController>(
            camera, static_cast<float>(WINDOW_WIDTH),
            static_cast<float>(WINDOW_HEIGHT));

    window->RegisterResizeCallback([&](int width, int height) {
        auto aspect = static_cast<float>(width) / static_cast<float>(height);
        glViewport(0, 0, width, height);
        camera->data.aspect = aspect;
        camera->data.width = FRUSTUM_SIZE * aspect;
        camera->data.height = FRUSTUM_SIZE;

        if (camera_controller != nullptr) {
            camera_controller->OnResizeCallback(width, height);
        }
    });

    window->RegisterKeyboardCallback([&](int key, int action, int modifier) {
        if (camera_controller != nullptr) {
            camera_controller->OnKeyCallback(key, action, modifier);
        }
    });

    window->RegisterMouseButtonCallback(
        [&](int button, int action, double x, double y) {
            if (camera_controller != nullptr) {
                camera_controller->OnMouseButtonCallback(button, action, x, y);
            }
        });

    window->RegisterMouseMoveCallback([&](double x, double y) {
        if (camera_controller != nullptr) {
            camera_controller->OnMouseMoveCallback(x, y);
        }
    });

    window->RegisterScrollCallback([&](double xOff, double yOff) {
        if (camera_controller != nullptr) {
            camera_controller->OnScrollCallback(xOff, yOff);
        }
    });

    while (window->active()) {
        ::utils::Clock::Tick();

        window->Begin();
        camera_controller->Update(::utils::Clock::GetTimeStep());

        debug_drawer->DrawLine({0.0F, 0.0F, 0.0F}, {5.0F, 0.0F, 0.0F},
                               {1.0F, 0.0F, 0.0F});
        debug_drawer->DrawLine({0.0F, 0.0F, 0.0F}, {0.0F, 5.0F, 0.0F},
                               {0.0F, 1.0F, 0.0F});
        debug_drawer->DrawLine({0.0F, 0.0F, 0.0F}, {0.0F, 0.0F, 5.0F},
                               {0.0F, 0.0F, 1.0F});

        Pose box_pose(Vec3(1.0F, 1.0F, 1.0F), Quat());
        debug_drawer->DrawBox({0.1F, 0.15F, 0.2F}, box_pose,
                              {1.0F, 1.0F, 1.0F});

        Pose sphere_pose(Vec3(1.0F, -1.0F, 1.0F), Quat());
        debug_drawer->DrawSphere(0.2F, sphere_pose, {0.1F, 0.1F, 0.8F});

        Pose cylinder_pose(Vec3(-1.0F, 1.0F, 1.0F), Quat());
        debug_drawer->DrawCylinder(0.2F, 0.2F, cylinder_pose,
                                   {0.1F, 0.8F, 0.1F});

        debug_drawer->Render(*camera);

#if defined(RENDERER_IMGUI)
        ImGui::Begin("Debug Drawer");
        /// NOLINTNEXTLINE
        ImGui::Text("num lines: {%zu}", debug_drawer->num_lines());
        /// NOLINTNEXTLINE
        ImGui::Text("num drawcalls: {%zu}", debug_drawer->num_drawcalls());
        debug_drawer->ClearCounters();

        ImGui::End();
#endif

        window->End();

        ::utils::Clock::Tock();
    }

    return 0;
}
