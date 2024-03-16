#version 330 core
layout (location = 0) in vec3 Pos;
out vec2 uv;

void main() {
    uv = (Pos.xy + 1.0) * 0.5;
    gl_Position = vec4(Pos, 1.0);
}
