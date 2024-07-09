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
            std::make_shared<FreeCamera>(cameraPos, up, YAW, PITCH, ROLL);

    std::string blackBodyTex = "../image/blackbody.png";
    renderer.loadTextures(blackBodyTex);

    Shader rayMarchShader = Shader("../shader/blackhole.vert", "../shader/blackhole.frag");
    Shader postProcessShader = Shader("../shader/framebuffer.vert", "../shader/framebuffer.frag");

    Shader brightPassShader = Shader("../shader/framebuffer.vert", "../shader/light.frag");
    Shader blurShader = Shader("../shader/framebuffer.vert", "../shader/blur.frag");

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

    renderer.draw(rayMarchShader, brightPassShader, blurShader, postProcessShader);

    return 0;
}
