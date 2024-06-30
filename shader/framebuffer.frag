#version 330 core
out vec4 fragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;

void main()
{
    fragColor = vec4(vec3(texture(screenTexture, TexCoord)), 1.0);
}