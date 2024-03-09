#version 330

uniform vec2 resolution;
uniform float fovScale = 1.0;

float cameraRoll = 0.0;

out vec4 fragColor;

vec3 traceColor(vec3 pos, vec3 dir) {
    vec3 color = vec3(0.0);
}

void main() {
    mat3 view;
    vec3 cameraPos;
    vec2 mouse = clamp(vec2(mouseX, mouseY) / resolution.xy, 0.0, 1.0) - 0.5;
    cameraPos = vec3(-cos(mouse.x * 10.0) * 15.0, mouse.y * 30.0, sin(mouse.x * 10.0) * 15.0);

    vec3 target = vec3(0.0, 0.0, 0.0);
    view = lookat(cameraPos, target, radians(cameraRoll));

    vec2 uv = gl_FragCoord.xy / resolution.xy - vec2(0.5);
    uv.x *= resolution.x / resolution.y;

    vec3 dir = normalize(vec3(-uv.x * fovScale, uv.y * fovScale, 1.0));
    vec3 pos = cameraPos;
    dir = view * dir;

    fragColor.rgb = rayMarch(pos, dir);
}
