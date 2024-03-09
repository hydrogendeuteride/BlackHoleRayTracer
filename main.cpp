#include <iostream>
#include <glad/glad.h>
#include "shader.h"
#include <GLFW/glfw3.h>

static constexpr int SCR_WIDTH = 1920;
static constexpr int SCR_HEIGHT = 1080;

static float mouseX, mouseY;

void mouseCallback(GLFWwindow *window, double x, double y) {
    static float lastX = 400.0f;
    static float lastY = 300.0f;
    static float yaw = 0.0f;
    static float pitch = 0.0f;
    static float firstMouse = true;

    mouseX = (float)x;
    mouseY = (float)y;
}

void frameBufferSizeCallback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "BlackHole", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    glfwSetWindowPos(window, 0, 0);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);

    Shader shader("../shader/blackhole.vert", "../shader/blackhole.frag");

    while (!glfwWindowShouldClose(window))
    {

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
