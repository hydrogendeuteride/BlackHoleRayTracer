#version 330 core
layout (location = 0) in vec3 Pos;

out vec2 TexCoord;

void main() {
    gl_Position = vec4(Pos, 1.0);
}
