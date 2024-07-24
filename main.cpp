#include <emscripten/emscripten.h>
#include <emscripten/html5_webgl.h>
#include "render.h"
#include "cameraBase.h"
#include "orbitCamera.h"
#include "freeCamera.h"

static constexpr int SCR_WIDTH = 1920;
static constexpr int SCR_HEIGHT = 1080;

int main()
{
    Render renderer(SCR_WIDTH, SCR_HEIGHT);

    glm::vec3 target = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 cameraPos = glm::vec3 (0.0, 0.0, 15.0);
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
    renderer.loadTextures(blackBodyTex, cubeMapTex);

    auto rayMarchShader= std::make_unique<Shader>("shader/blackhole.vert", "shader/blackhole.frag");
    auto postProcessShader = std::make_unique<Shader>("shader/framebuffer.vert", "shader/framebuffer.frag");

    auto brightPassShader = std::make_unique<Shader>("shader/framebuffer.vert", "shader/light.frag");
    auto blurShader = std::make_unique<Shader>("shader/framebuffer.vert", "shader/blur.frag");

    renderer.cameraSetup(orbitCamera);

    renderer.setCameraSwitchCallback([&]{
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

    renderer.draw(std::move(rayMarchShader), std::move(brightPassShader),
                  std::move(blurShader), std::move(postProcessShader));

    emscripten_set_main_loop_arg(Render::mainLoopWrapper, &renderer, 0, 1);

    return 0;
}
