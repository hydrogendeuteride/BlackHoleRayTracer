#version 430 core

precision highp float;

uniform vec2 resolution;

uniform float time;

uniform int integrationType;
uniform bool disk;
uniform int accretionDiskOrbit;
uniform bool dopplerShift;
uniform bool gravitationalRedShift;
uniform bool beaming;
uniform bool realisticTemperature;
uniform float accretionTemp;

uniform vec3 cameraPos;
uniform mat4 view;

float diskHeight = 0.3;
float diskDensityV = 1.0;
float diskDensityH = 2.0;
float diskNoiseScale = 1.0;
float diskSpeed = 0.5;

int diskNoiseLOD = 6;

float steps = 0.1;

int iteration = 512;    //256 is sufficient

const float G = 1.0;
const float M = 1.0;
const float c = 1.0;

const float TEMP_RANGE = 39000.0; //1000K~40000K

out vec4 fragColor;

uniform sampler2D blackbody;
uniform samplerCube cubemap;

//	Simplex 3D Noise
//	by Ian McEwan, Ashima Arts
//  https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
vec4 permute(vec4 x){ return mod(((x*34.0)+1.0)*x, 289.0); }
vec4 taylorInvSqrt(vec4 r){ return 1.79284291400159 - 0.85373472095314 * r; }

float snoise(vec3 v){
    const vec2  C = vec2(1.0/6.0, 1.0/3.0);
    const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

    // First corner
    vec3 i  = floor(v + dot(v, C.yyy));
    vec3 x0 =   v - i + dot(i, C.xxx);

    // Other corners
    vec3 g = step(x0.yzx, x0.xyz);
    vec3 l = 1.0 - g;
    vec3 i1 = min(g.xyz, l.zxy);
    vec3 i2 = max(g.xyz, l.zxy);

    //  x0 = x0 - 0. + 0.0 * C
    vec3 x1 = x0 - i1 + 1.0 * C.xxx;
    vec3 x2 = x0 - i2 + 2.0 * C.xxx;
    vec3 x3 = x0 - 1. + 3.0 * C.xxx;

    // Permutations
    i = mod(i, 289.0);
    vec4 p = permute(permute(permute(
    i.z + vec4(0.0, i1.z, i2.z, 1.0))
    + i.y + vec4(0.0, i1.y, i2.y, 1.0))
    + i.x + vec4(0.0, i1.x, i2.x, 1.0));

    // Gradients
    // ( N*N points uniformly over a square, mapped onto an octahedron.)
    float n_ = 1.0/7.0;// N=7
    vec3  ns = n_ * D.wyz - D.xzx;

    vec4 j = p - 49.0 * floor(p * ns.z *ns.z);//  mod(p,N*N)

    vec4 x_ = floor(j * ns.z);
    vec4 y_ = floor(j - 7.0 * x_);// mod(j,N)

    vec4 x = x_ *ns.x + ns.yyyy;
    vec4 y = y_ *ns.x + ns.yyyy;
    vec4 h = 1.0 - abs(x) - abs(y);

    vec4 b0 = vec4(x.xy, y.xy);
    vec4 b1 = vec4(x.zw, y.zw);

    vec4 s0 = floor(b0)*2.0 + 1.0;
    vec4 s1 = floor(b1)*2.0 + 1.0;
    vec4 sh = -step(h, vec4(0.0));

    vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy;
    vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww;

    vec3 p0 = vec3(a0.xy, h.x);
    vec3 p1 = vec3(a0.zw, h.y);
    vec3 p2 = vec3(a1.xy, h.z);
    vec3 p3 = vec3(a1.zw, h.w);

    //Normalise gradients
    vec4 norm = taylorInvSqrt(vec4(dot(p0, p0), dot(p1, p1), dot(p2, p2), dot(p3, p3)));
    p0 *= norm.x;
    p1 *= norm.y;
    p2 *= norm.z;
    p3 *= norm.w;

    // Mix final noise value
    vec4 m = max(0.6 - vec4(dot(x0, x0), dot(x1, x1), dot(x2, x2), dot(x3, x3)), 0.0);
    m = m * m;
    return 42.0 * dot(m*m, vec4(dot(p0, x0), dot(p1, x1),
    dot(p2, x2), dot(p3, x3)));
}

vec3 toSpherical(vec3 pos){
    float rho = sqrt((pos.x * pos.x) + (pos.y * pos.y) + (pos.z * pos.z));
    float theta = atan(pos.z, pos.x);
    float phi = asin(pos.y /rho);
    return vec3(rho, theta, phi);
}

vec4 getBlackBodyColor(float temp)
{
    float x_coord = clamp((temp - 1000) / TEMP_RANGE, 0.0, 1.0);
    vec2 texCoord = vec2(x_coord, 0.5);
    vec4 color = texture2D(blackbody, texCoord);
    return color;
}

float calculateRedShift(vec3 pos)
{
    float dist = sqrt(dot(pos, pos));
    if (dist < 1.0f)
    {
        return 0.0f;
    }
    float redshift = sqrt(1.0f - 1.0f/dist) - 1.0f;

    redshift = (1 / (1.0 + redshift));
    return redshift;
}

float calculateDopplerEffect(vec3 pos, vec3 viewDir)
{
    vec3 vel;
    float r = length(pos);
    if (r < 1.0)
    {
        vel = vec3(0.0f);
        return 1.0f;
    }

    float velMag;
    if (accretionDiskOrbit == 0)
    {
        velMag = -sqrt((G * M / r));    //non relativistic speed
    }
    else if (accretionDiskOrbit == 1)
    {
        velMag = -sqrt((G * M / r) * (1.0 - 3.0 * G * M / (r * c * c)));    //relativistic speed
    }

    vec3 velDir = normalize(cross(vec3(0.0, 1.0, 0.0), pos));
    vel = velDir * velMag;

    vec3 beta_s = vel / c;

    float gamma = 1.0 / sqrt(1.0 - dot(beta_s, beta_s));
    float dopplerShift = gamma * (1.0 + dot(vel, normalize(viewDir)));

    return dopplerShift;
}

float calculateRealisticTemperature(vec3 pos, float baseTemp) {
    float radius = length(pos);
    return baseTemp * pow(radius, -0.75);
}

void diskRender(vec3 pos, inout vec4 color, inout float alpha, vec3 viewDir){
    float innerRadius = 3.0;
    float outerRadius = 9.0;

    float density = max(0.0, 1.0 - length(pos / vec3(outerRadius, diskHeight, outerRadius)));

    if (density < 0.001){
        return;
    }

    density *= pow(1.0 - abs(pos.y) / diskHeight, diskDensityV);

    density *= smoothstep(innerRadius, innerRadius * 1.1, length(pos));

    if (density < 0.001){
        return;
    }

    vec3 sphericalCoord =  toSpherical(pos);

    sphericalCoord.y *= 2.0;
    sphericalCoord.z *= 4.0;

    density *= 1.0 / pow(sphericalCoord.x, diskDensityH);
    density *= 16000.0;

    float noise = 1.0;
    for (int i = 0; i < diskNoiseLOD; ++i){
        noise *= 0.5 * snoise(sphericalCoord * pow(i, 2) * diskNoiseScale) + 0.5;
        if (i % 2 == 0) {
            sphericalCoord.y -= time * diskSpeed;
        } else {
            sphericalCoord.y += time * diskSpeed;
        }
    }

    float redshift = calculateRedShift(pos);
    float doppler = calculateDopplerEffect(pos, viewDir);

    float accretionTempMod = accretionTemp;

    if (realisticTemperature)
        accretionTempMod = calculateRealisticTemperature(pos, accretionTempMod);

    if(dopplerShift)
        accretionTempMod /= doppler;

    if (gravitationalRedShift)
        accretionTempMod /= redshift;

    vec4 dustColor = getBlackBodyColor(accretionTempMod *  redshift) * 0.5;

    color += density * dustColor * alpha * abs(noise);

    if (gravitationalRedShift)
        color /= redshift;

    if (beaming)
        color /= doppler * doppler * doppler;
}

vec3 acceleration(float h2, vec3 pos){
    float r2 = dot(pos, pos);
    float r5 = pow(r2, 2.5);
    vec3 acc = -1.5 * h2 * pos / r5;
    return acc;
}

void verlet(inout vec3 pos, float h2, inout vec3 dir, float dt){
    vec3 a = acceleration(h2, pos);
    vec3 pos_new = pos + dir * dt + 0.5 * a * dt * dt;
    vec3 a_new = acceleration(h2, pos_new);

    dir += 0.5 * (a + a_new) * dt;
    pos = pos_new;
}

void rk4(inout vec3 pos, float h2, inout vec3 dir, float dt) {
    vec3 k1_pos = dir;
    vec3 k1_vel = acceleration(h2, pos);

    vec3 k2_pos = dir + 0.5 * k1_vel * dt;
    vec3 k2_vel = acceleration(h2, pos + 0.5 * k1_pos * dt);

    vec3 k3_pos = dir + 0.5 * k2_vel * dt;
    vec3 k3_vel = acceleration(h2, pos + 0.5 * k2_pos * dt);

    vec3 k4_pos = dir + k3_vel * dt;
    vec3 k4_vel = acceleration(h2, pos + k3_pos * dt);

    vec3 pos_new = pos + (1.0 / 6.0) * (k1_pos + 2.0 * k2_pos + 2.0 * k3_pos + k4_pos) * dt;
    vec3 dir_new = dir + (1.0 / 6.0) * (k1_vel + 2.0 * k2_vel + 2.0 * k3_vel + k4_vel) * dt;

    pos = pos_new;
    dir = dir_new;
}

vec4 rayMarch(vec3 pos, vec3 dir) {
    vec4 color = vec4(0.0);
    float alpha = 1.0;

    vec3 h = cross(pos, dir);
    float h2 = dot(h, h);

    for (int i = 0; i < iteration; ++i){
        if (integrationType == 0)
            verlet(pos, h2, dir, steps);
        else
            rk4(pos, h2, dir, steps);

        if (dot(pos, pos) < 1.0){
            return color;
        }

        if (disk)
            diskRender(pos, color, alpha, dir);
    }

    vec4 skyColor = texture(cubemap, normalize(dir));
    color += skyColor;

    return color;
}

void main(){
    vec2 uv = (2.0 * gl_FragCoord.xy - resolution.xy) / resolution.y;

    vec3 dir = normalize(vec3(uv, 1.0));
    vec3 pos = cameraPos;

    dir = (view * vec4(dir, 0.0)).xyz;
    fragColor = rayMarch(pos, dir);
}