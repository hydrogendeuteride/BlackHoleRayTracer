#include <iostream>
#include <glad/glad.h>
#include "shader.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

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
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);

    std::vector<float> vertices = {
            //pos
            1.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
    };

    std::vector<int> indicies = {
            0, 1, 3,
            1, 2, 3
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof (float), static_cast<void*>(vertices.data()), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof (unsigned int), static_cast<void*>(indicies.data()), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (float), (void*)0 );
    glEnableVertexAttribArray(0);

    Shader shader("../shader/blackhole.vert", "../shader/blackhole.frag");

    shader.setVec2("resolution", 1920.0f, 1080.0f);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int w, h, ch;
    unsigned char* data = stbi_load("../image/blackbody.png", &w, &h, &ch, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    float lastTime = 0.0f;
    float currentTime = 0.0f;
    float fps = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        currentTime = (float )glfwGetTime();
        fps = 1.0f / (currentTime - lastTime);
        lastTime = currentTime;
        std::cout << fps << "\n";

        shader.use();
        shader.setVec2("resolution", 1920.0f, 1080.0f);
        shader.setFloat("time", (float )glfwGetTime());
        shader.setFloat("mouseX", mouseX);
        shader.setFloat("mouseY", mouseY);

        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader.setInt("blackbody", 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
