#version 330 core
out vec4 fragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;
uniform sampler2D bloomBlur;

uniform bool bloom;
uniform float bloomStrength;
uniform int toneMapping;

vec3 ACESFilm(vec3 x) {
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0.0, 1.0);
}

vec3 HableToneMapping(vec3 x) {
    float A = 0.15;
    float B = 0.50;
    float C = 0.10;
    float D = 0.20;
    float E = 0.02;
    float F = 0.30;
    return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

vec3 ReinhardToneMapping(vec3 color) {
    return color / (color + vec3(1.0));
}

void main()
{
    float gamma = 2.2;
    vec4 hdrColor = clamp(texture(screenTexture, TexCoord), 0.0, 5.0);
    vec4 bloomColor = clamp(texture(bloomBlur, TexCoord), 0.0, 5.0) * bloomStrength;

    if (bloom)
        hdrColor += bloomColor;

    vec3 mapped;
    if (toneMapping == 0)
        mapped = HableToneMapping(hdrColor.rgb);   //Hable tone mapping
    if (toneMapping == 1)
        mapped = ACESFilm(hdrColor.rgb);         //ACES tone mapping
    if (toneMapping == 2)
        mapped = ReinhardToneMapping(hdrColor.rgb);
    if (toneMapping == 3)
        mapped = hdrColor.rgb/1.0;

    mapped = pow(mapped, vec3(1.0 / gamma));
    fragColor = vec4(mapped, hdrColor.a);
}