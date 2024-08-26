#include "render.h"
#include "cameraBase.h"
#include "orbitCamera.h"
#include "freeCamera.h"
#include "computeShader.h"

static constexpr int RENDERWIDTH = 1920;
static constexpr int RENDERHEIGHT = 1080;

int main()
{
    Render renderer(RENDERWIDTH, RENDERHEIGHT);

    glm::vec3 target = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 15.0);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    std::shared_ptr<OrbitCamera> orbitCamera =
            std::make_shared<OrbitCamera>(cameraPos, up, YAW, PITCH, target);
    std::shared_ptr<FreeCamera> freeCamera =
            std::make_shared<FreeCamera>(-cameraPos, up, YAW - 90, PITCH, ROLL);

    std::string blackBodyTex = "image/blackbody.png";
    std::vector<std::string> cubeMapTex
    {
        "image/skybox_8k/right.png",
        "image/skybox_8k/left.png",
        "image/skybox_8k/up.png",
        "image/skybox_8k/down.png",
        "image/skybox_8k/front.png",
        "image/skybox_8k/back.png"
    };

    std::vector<std::string> cubeMapTexHDR
    {
        "image/skybox_8k/right.hdr",
        "image/skybox_8k/left.hdr",
        "image/skybox_8k/up.hdr",
        "image/skybox_8k/down.hdr",
        "image/skybox_8k/front.hdr",
        "image/skybox_8k/back.hdr"
    };

    renderer.loadTextures(blackBodyTex, cubeMapTex, cubeMapTexHDR);

    auto rayMarchShader = std::make_unique<Shader>("shader/blackhole.vert", "shader/basic.frag");
    auto postProcessShader = std::make_unique<Shader>("shader/framebuffer.vert", "shader/framebuffer.frag");

    auto brightPassShader = std::make_unique<Shader>("shader/framebuffer.vert", "shader/light.frag");
    auto blurShader = std::make_unique<Shader>("shader/framebuffer.vert", "shader/blur.frag");

    auto computeShader = std::make_unique<ComputeShader>("shader/blackhole.comp");

    auto screenShader = std::make_unique<Shader>("shader/screen.vert", "shader/screen.frag");

    renderer.cameraSetup(orbitCamera);

    renderer.setCameraSwitchCallback([&] {
        static bool useOrbit = true;
        if (useOrbit)
        {
            renderer.cameraSetup(freeCamera);
        }
        else
        {
            renderer.cameraSetup(orbitCamera);
        }
        useOrbit = !useOrbit;
    });

    renderer.setImGui("fonts/Hack-Regular.ttf", 24.0f);

    renderer.draw(std::move(computeShader),
                  std::move(brightPassShader),
                  std::move(blurShader),
                  std::move(postProcessShader),
                  std::move(screenShader));

    return 0;
}
