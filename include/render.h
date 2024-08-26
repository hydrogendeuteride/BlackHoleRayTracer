#ifndef BLACKHOLERAYTRACER_RENDER_H
#define BLACKHOLERAYTRACER_RENDER_H

#include "orbitCamera.h"
#include "freeCamera.h"
#include "shader.h"
#include "computeShader.h"
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

    void cameraSetup(std::shared_ptr<CameraBase> newCamera);

    void draw(std::unique_ptr<ComputeShader> computeShader,
              std::unique_ptr<Shader> BrightPassShader,
              std::unique_ptr<Shader> BlurShader,
              std::unique_ptr<Shader> PostProcessShader,
              std::unique_ptr<Shader> screenShader);

    void setCameraSwitchCallback(std::function<void()> callback);

    void loadTextures(std::string& blackBody, std::vector<std::string>& cubeMap, std::vector<std::string> &hdrCubeMap);

    void setImGui(std::string fontPath, float fontSize);

private:
    void frameBufferSizeCallback(int width, int height);

    void mouseMovementCallback(double xPosIn, double YPosIn);

    void mouseButtonCallback(int button, int action, int mods);

    void processInput(GLFWwindow *pWindow);

    void initQuad();

    void initResolution();

    void initBloom();

    void initComputeShader();

    void toggleFullScreen();

    std::pair<int, int> updateViewPort() const;

    GLFWwindow *window;

    std::shared_ptr<CameraBase> camera;

    bool firstMouse = true;

    int renderWidth, renderHeight;

    float lastX = static_cast<float>(renderWidth) / 2.0f;
    float lastY = static_cast<float>(renderHeight) / 2.0f;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    glm::vec2 lastMousePosition;
    bool isDragging = false;

    unsigned int lightFBO;
    unsigned int colorBuffers[2];
    unsigned int pingpongFBO[2];
    unsigned int pingpongColorBuffers[2];

    unsigned int resolutionFBO;
    unsigned int resolutionTexture;

    int isFullScreen = 0;
    int windowedXPos, windowedYPos;
    int windowedWidth, windowedHeight;

    unsigned int quadVAO, quadVBO, quadEBO;

    std::function<void()> cameraSwitchCallback;

    unsigned int blackBodyTexture, cubeMapTexture;

    unsigned int computeTexture;

    ImFont* font;
    BlackHoleSettings bh;
    RendererSettings rs;
    bool hudOFF = true;

    std::chrono::steady_clock::time_point lastPressTimeC = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point lastPressTimeH = std::chrono::steady_clock::now();

    std::unique_ptr<Shader> brightPassShader, blurShader, postProcessShader;
    std::unique_ptr<ComputeShader> computeShader;
};

#endif //BLACKHOLERAYTRACER_RENDER_H
