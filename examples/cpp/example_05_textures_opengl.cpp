#include <glad/gl.h>

#include <utils/logging.hpp>

#include <renderer/engine/graphics/window_t.hpp>
#include <renderer/backend/graphics/opengl/program_opengl_t.hpp>
#include <renderer/backend/graphics/opengl/vertex_buffer_opengl_t.hpp>
#include <renderer/backend/graphics/opengl/vertex_array_opengl_t.hpp>
#include <renderer/backend/graphics/opengl/texture_opengl_t.hpp>

#if defined(RENDERER_IMGUI)
#include <imgui.h>
#endif  // RENDERER_IMGUI

constexpr const char* VERT_SHADER_SRC = R"(
    #version 330 core

    layout (location = 0) in vec2 position;
    layout (location = 1) in vec2 texcoord;

    out vec2 tex_coord;

    void main() {
        gl_Position = vec4(position, 0.0f, 1.0f);
        tex_coord = texcoord;
    }
)";

constexpr const char* FRAG_SHADER_SRC = R"(
    #version 330 core

    in vec2 tex_coord;
    out vec4 output_color;

    uniform sampler2D u_texture;

    void main() {
        output_color = texture(u_texture, tex_coord);
    }
)";

auto get_wrapping_mode_idx(const renderer::eTextureWrap& tex_wrap) -> uint32_t;

auto get_wrapping_mode_from_idx(uint32_t idx) -> renderer::eTextureWrap;

// NOLINTNEXTLINE
auto main() -> int {
    // NOLINTNEXTLINE
    auto IMAGE_PATH = ::renderer::RESOURCES_PATH + "images/container.jpg";

    constexpr int WINDOW_WIDTH = 1024;
    constexpr int WINDOW_HEIGHT = 768;
    constexpr auto WINDOW_API = ::renderer::eWindowBackend::TYPE_GLFW;

    renderer::WindowConfig win_config;
    win_config.backend = WINDOW_API;
    win_config.width = WINDOW_WIDTH;
    win_config.height = WINDOW_HEIGHT;
    win_config.title = "Example 05 - OpenGL Textures";
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
    /*|      pos       texture  */
        -0.5F, -0.5F, 0.0F, 0.0F, // NOLINT
         0.5F, -0.5F, 2.0F, 0.0F, // NOLINT
         0.5F,  0.5F, 2.0F, 2.0F, // NOLINT
        -0.5F,  0.5F, 0.0F, 2.0F // NOLINT
    };
    constexpr uint32_t NUM_INDICES = 6;

    // NOLINTNEXTLINE
    uint32_t buffer_indices[] = {
        0, 1, 2,  // first triangle
        0, 2, 3  // second triangle
    };
    // clang-format on

    ::renderer::opengl::OpenGLBufferLayout layout = {
        {"position", renderer::eElementType::FLOAT_2, false},
        {"texcoord", renderer::eElementType::FLOAT_2, false}};

    auto vbo = std::make_shared<renderer::opengl::OpenGLVertexBuffer>(
        layout, renderer::eBufferUsage::STATIC,
        static_cast<uint32_t>(sizeof(buffer_data)), buffer_data);

    auto ibo = std::make_shared<renderer::opengl::OpenGLIndexBuffer>(
        renderer::eBufferUsage::STATIC, NUM_INDICES, buffer_indices);

    auto vao = std::make_shared<renderer::opengl::OpenGLVertexArray>();
    vao->AddVertexBuffer(vbo);
    vao->SetIndexBuffer(ibo);

    auto texture =
        std::make_shared<renderer::opengl::OpenGLTexture>(IMAGE_PATH.c_str());

    while (window->active()) {
        window->Begin();

        // Render our textured triangle
        {
            program->Bind();
            texture->Bind();
            vao->Bind();

            glDrawElements(GL_TRIANGLES, NUM_INDICES, GL_UNSIGNED_INT, nullptr);

            vao->Unbind();
            texture->Unbind();
            program->Unbind();
        }

#if defined(RENDERER_IMGUI)
        ImGui::Begin("Options");
        {
            // Options for wrap-u
            std::array<const char*, 4> combo_items = {
                "repeat", "repeat_mirror", "clamp_to_edge", "clamp_to_border"};
            static uint32_t s_CurrentIndex =
                get_wrapping_mode_idx(texture->wrap_mode_u());
            const char* combo_preview_value = combo_items.at(s_CurrentIndex);
            if (ImGui::BeginCombo("Wrapping-mode-u", combo_preview_value)) {
                for (uint32_t i = 0; i < combo_items.size(); i++) {
                    bool is_selected = (s_CurrentIndex == i);
                    if (ImGui::Selectable(combo_items.at(i), is_selected)) {
                        s_CurrentIndex = i;
                    }

                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
            }

            auto new_wrapping_mode = get_wrapping_mode_from_idx(s_CurrentIndex);
            if (new_wrapping_mode != texture->wrap_mode_u()) {
                // It's a new one, so set it :)
                texture->SetWrapModeU(new_wrapping_mode);
            }
        }
        {
            // Options for wrap-v
            std::array<const char*, 4> combo_items = {
                "repeat", "repeat_mirror", "clamp_to_edge", "clamp_to_border"};
            static uint32_t s_CurrentIndex =
                get_wrapping_mode_idx(texture->wrap_mode_v());
            const char* combo_preview_value = combo_items.at(s_CurrentIndex);
            if (ImGui::BeginCombo("Wrapping-mode-v", combo_preview_value)) {
                for (uint32_t i = 0; i < combo_items.size(); i++) {
                    bool is_selected = (s_CurrentIndex == i);
                    if (ImGui::Selectable(combo_items.at(i), is_selected)) {
                        s_CurrentIndex = i;
                    }

                    if (is_selected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
            }

            auto new_wrapping_mode = get_wrapping_mode_from_idx(s_CurrentIndex);
            if (new_wrapping_mode != texture->wrap_mode_v()) {
                // It's a new one, so set it :)
                texture->SetWrapModeV(new_wrapping_mode);
            }
        }
        {
            // Options for border color
            static Vec4 s_Color = texture->border_color();
            ImGui::ColorEdit3("border-color", s_Color.data());
            if (s_Color != texture->border_color()) {
                texture->SetBorderColor(s_Color);
            }
        }
        ImGui::End();
#endif  // RENDERER_IMGUI

        window->End();
    }

    return 0;
}

auto get_wrapping_mode_idx(const renderer::eTextureWrap& tex_wrap) -> uint32_t {
    switch (tex_wrap) {
        case renderer::eTextureWrap::REPEAT:
            return 0;
        case renderer::eTextureWrap::REPEAT_MIRROR:
            return 1;
        case renderer::eTextureWrap::CLAMP_TO_EDGE:
            return 2;
        case renderer::eTextureWrap::CLAMP_TO_BORDER:
            return 3;
        default:
            return 0;
    }
}

auto get_wrapping_mode_from_idx(uint32_t idx) -> renderer::eTextureWrap {
    switch (idx) {
        case 0:
            return renderer::eTextureWrap::REPEAT;
        case 1:
            return renderer::eTextureWrap::REPEAT_MIRROR;
        case 2:
            return renderer::eTextureWrap::CLAMP_TO_EDGE;
        case 3:
            return renderer::eTextureWrap::CLAMP_TO_BORDER;
        default:
            return renderer::eTextureWrap::REPEAT;
    }
}
