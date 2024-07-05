#version 330 core
out vec4 fragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;

vec3 ACESFilm(vec3 x) {
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0.0, 1.0);
}

void main()
{
    float gamma = 2.2;
    vec3 hdrColor = texture(screenTexture, TexCoord).rgb;
    vec3 mapped = ACESFilm(hdrColor);
    mapped = pow(mapped, vec3(1.0 / gamma));
    fragColor = vec4(mapped, 1.0);
}