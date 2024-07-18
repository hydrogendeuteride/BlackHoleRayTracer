#ifndef BLACKHOLERAYTRACER_RENDER_H
#define BLACKHOLERAYTRACER_RENDER_H

#include "orbitCamera.h"
#include "freeCamera.h"
#include "shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <functional>
#include "texture.h"

#include "imguiWidget.h"

class Render
{
public:
    Render(int scrWidth, int scrHeight);

    void cameraSetup(std::shared_ptr<CameraBase> newCamera);

    void draw(Shader rayMarchShader, Shader brightPassShader, Shader blurShader, Shader postProcessShader);

    void setCameraSwitchCallback(std::function<void()> callback);

    void loadTextures(std::string& blackBody, std::vector<std::string>& cubeMap);

    void setImGui(std::string fontPath, float fontSize);

private:
    void frameBufferSizeCallback(int width, int height);

    void scrollCallback(float xOffset, float yOffset);

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
};

#endif //BLACKHOLERAYTRACER_RENDER_H
