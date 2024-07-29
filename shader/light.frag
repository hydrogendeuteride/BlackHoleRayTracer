#version 430

in vec2 TexCoord;

uniform sampler2D sceneTexture;
uniform float threshold = 1.0;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

void main() {
    vec4 hdrColor = texture(sceneTexture, TexCoord);

    fragColor = hdrColor;

    float brightness = dot(hdrColor.rgb, vec3(0.213, 0.7152, 0.0722));
    if (brightness > threshold)
        brightColor = vec4(hdrColor.rgb, hdrColor.a);
    else
        brightColor = vec4(0.0, 0.0, 0.0, hdrColor.a);
}
