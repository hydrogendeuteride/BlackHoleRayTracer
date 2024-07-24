#ifndef BLACKHOLERAYTRACER_RENDER_H
#define BLACKHOLERAYTRACER_RENDER_H

#include "orbitCamera.h"
#include "freeCamera.h"
#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <functional>
#include <chrono>
#include "texture.h"

#include "imguiWidget.h"

class Render
{
public:
    Render(int scrWidth, int scrHeight);
    ~Render();

    void cameraSetup(std::shared_ptr<CameraBase> newCamera);

    void draw(std::unique_ptr<Shader> RayMarchShader, std::unique_ptr<Shader> BrightPassShader,
              std::unique_ptr<Shader> BlurShader, std::unique_ptr<Shader> PostProcessShader);

    void setCameraSwitchCallback(std::function<void()> callback);

    void loadTextures(std::string& blackBody, std::vector<std::string>& cubeMap);

    void setImGui(std::string fontPath, float fontSize);

    static void mainLoopWrapper(void* arg);

    void mainLoop();

private:
    void frameBufferSizeCallback(int width, int height);

    void mouseMovementCallback(double xPosIn, double YPosIn);

    void mouseButtonCallback(int button, int action, int mods);

    void processInput(GLFWwindow *pWindow);

    void initFrameBuffer();

    void initRayMarch();

    void initBloom();

    GLFWwindow *window;

    std::shared_ptr<CameraBase> camera;

    bool firstMouse = true;

    int SCRWIDTH, SCRHEIGHT;

    float lastX = static_cast<float>(SCRWIDTH) / 2.0f;
    float lastY = static_cast<float>(SCRHEIGHT) / 2.0f;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    glm::vec2 lastMousePosition;
    bool isDragging = false;

    unsigned int frameBuffer, texColorBuffer, rbo;

    unsigned int lightFBO;
    unsigned int colorBuffers[2];
    unsigned int pingpongFBO[2];
    unsigned int pingpongColorBuffers[2];

    unsigned int quadVAO, quadVBO, quadEBO;

    unsigned int VBO, VAO, EBO;

    std::function<void()> cameraSwitchCallback;

    unsigned int blackBodyTexture, cubeMapTexture;

    ImFont* font;
    BlackHoleSettings bh;
    RendererSettings rs;
    bool hudOFF = true;

    std::chrono::steady_clock::time_point lastPressTimeC = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point lastPressTimeH = std::chrono::steady_clock::now();

    std::unique_ptr<Shader> rayMarchShader, brightPassShader, blurShader, postProcessShader;

    float lastTime = 0.0;
};

#endif //BLACKHOLERAYTRACER_RENDER_H
