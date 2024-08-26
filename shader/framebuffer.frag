#version 430
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

vec3 rec2020ToDCIP3(vec3 color)
{
    const mat3 rec2020_to_dciP3 = mat3(
    1.2489, -0.1185, -0.1304,
    -0.0494, 1.1605, -0.1111,
    -0.0036, -0.0183, 1.0219
    );
    return rec2020_to_dciP3 * color;
}

void main()
{
    float gamma = 2.4;
    vec4 hdrColor = texture(screenTexture, TexCoord);
    vec4 bloomColor = texture(bloomBlur, TexCoord) * bloomStrength;

    if (bloom)
        hdrColor += bloomColor;

    hdrColor.xyz = rec2020ToDCIP3(hdrColor.xyz);

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