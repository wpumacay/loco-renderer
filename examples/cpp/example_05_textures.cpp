
#include <iostream>

#include <glad/gl.h>

#include <loco/renderer/window/window_t.hpp>
#include <loco/renderer/shader/program_t.hpp>
#include <loco/renderer/core/vertex_buffer_t.hpp>
#include <loco/renderer/core/vertex_array_t.hpp>
#include "loco/renderer/core/vertex_buffer_layout_t.hpp"
#include <loco/renderer/core/texture_t.hpp>

#if defined(LOCORENDERER_IMGUI)
#include <imgui.h>
#endif  // LOCORENDERER_IMGUI

constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 768;

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

auto get_wrapping_mode_idx(const loco::renderer::eTextureWrap& tex_wrap)
    -> uint32_t;

auto get_wrapping_mode_from_idx(uint32_t idx) -> loco::renderer::eTextureWrap;

auto main() -> int {
    // NOLINTNEXTLINE
    auto IMAGE_PATH =
        std::string(loco::renderer::RESOURCES_PATH) + "images/container.jpg";

    auto window =
        std::make_unique<loco::renderer::Window>(WINDOW_WIDTH, WINDOW_HEIGHT);

    window->RegisterKeyboardCallback([&](int key, int, int) {
        if (key == loco::renderer::keys::KEY_ESCAPE) {
            window->RequestClose();
        }
    });

    auto program = std::make_unique<loco::renderer::Program>(
        "basic_2d", VERT_SHADER_SRC, FRAG_SHADER_SRC);

    // clang-format off
    // NOLINTNEXTLINE
    float buffer_data[] = {
    /*|      pos       texture  */
        -0.5F, -0.5F, 0.0F, 0.0F, // NOLINT
         0.5F, -0.5F, 2.0F, 0.0F, // NOLINT
         0.5F,  0.5F, 2.0F, 2.0F, // NOLINT
        -0.5F,  0.5F, 0.0F, 2.0F // NOLINT
    };
    constexpr uint32_t NUM_VERTICES = 6;

    // NOLINTNEXTLINE
    uint32_t buffer_indices[] = {
        0, 1, 2,  // first triangle
        0, 2, 3  // second triangle
    };
    // clang-format on

    loco::renderer::BufferLayout layout = {
        {"position", loco::renderer::eElementType::FLOAT_2, false},
        {"texcoord", loco::renderer::eElementType::FLOAT_2, false}};

    auto vbo = std::make_unique<loco::renderer::VertexBuffer>(
        layout, loco::renderer::eBufferUsage::STATIC,
        static_cast<uint32_t>(sizeof(buffer_data)), buffer_data);

    auto ibo = std::make_unique<loco::renderer::IndexBuffer>(
        loco::renderer::eBufferUsage::STATIC, NUM_VERTICES, buffer_indices);

    auto vao = std::make_unique<loco::renderer::VertexArray>();
    vao->AddVertexBuffer(std::move(vbo));
    vao->SetIndexBuffer(std::move(ibo));

    auto texture =
        std::make_unique<loco::renderer::Texture>(IMAGE_PATH.c_str());

    while (window->active()) {
        window->Begin();

        // Render our textured triangle
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

#if defined(LOCORENDERER_IMGUI)
        ImGui::ShowDemoWindow();
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
#endif  // LOCORENDERER_IMGUI

        window->End();
    }

    return 0;
}

auto get_wrapping_mode_idx(const loco::renderer::eTextureWrap& tex_wrap)
    -> uint32_t {
    switch (tex_wrap) {
        case loco::renderer::eTextureWrap::REPEAT:
            return 0;
        case loco::renderer::eTextureWrap::REPEAT_MIRROR:
            return 1;
        case loco::renderer::eTextureWrap::CLAMP_TO_EDGE:
            return 2;
        case loco::renderer::eTextureWrap::CLAMP_TO_BORDER:
            return 3;
    }
}

auto get_wrapping_mode_from_idx(uint32_t idx) -> loco::renderer::eTextureWrap {
    switch (idx) {
        case 0:
            return loco::renderer::eTextureWrap::REPEAT;
        case 1:
            return loco::renderer::eTextureWrap::REPEAT_MIRROR;
        case 2:
            return loco::renderer::eTextureWrap::CLAMP_TO_EDGE;
        case 3:
            return loco::renderer::eTextureWrap::CLAMP_TO_BORDER;
        default:
            return loco::renderer::eTextureWrap::REPEAT;
    }
}
