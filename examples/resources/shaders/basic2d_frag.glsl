#version 330 core

in vec2 f_texCoord;
out vec4 f_color;

uniform sampler2D u_texture;

void main() {
    f_color = texture(u_texture, f_texCoord);
}
