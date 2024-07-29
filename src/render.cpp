#include "render.h"

#include <utility>

Render::Render(int scrWidth, int scrHeight)
{
    SCRWIDTH = scrWidth;
    SCRHEIGHT = scrHeight;

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCRWIDTH, SCRHEIGHT, "BlackHoleRayTracer", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *w, int width, int height) {
        auto *win = static_cast<Render *>(glfwGetWindowUserPointer(w));
        win->frameBufferSizeCallback(width, height);
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow *w, double xOffset, double yOffset) {
        auto *win = static_cast<Render *>(glfwGetWindowUserPointer(w));
        win->mouseMovementCallback(xOffset, yOffset);
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow *w, int button, int action, int mods) {
        auto *win = static_cast<Render *>(glfwGetWindowUserPointer(w));
        win->mouseButtonCallback(button, action, mods);
    });

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);

    initQuad();
    initBloom();
    initComputeShader();
}

void Render::cameraSetup(std::shared_ptr<CameraBase> newCamera)
{
    camera = newCamera;
}

void Render::frameBufferSizeCallback(int width, int height)
{
    glViewport(0, 0, width, height);
}

void Render::mouseButtonCallback(int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
        {
            isDragging = true;
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            lastMousePosition = glm::vec2(mouseX, mouseY);
        }
        else if (action == GLFW_RELEASE)
        {
            isDragging = false;
        }
    }
}

void Render::mouseMovementCallback(double xPosIn, double yPosIn)
{
    auto xpos = static_cast<float>(xPosIn);
    auto ypos = static_cast<float>(yPosIn);

    if (isDragging)
    {
        glm::vec2 currentMousePosition = glm::vec2(xpos, ypos);
        glm::vec2 mouseDelta = currentMousePosition - lastMousePosition;

        camera->processMouseMovement(mouseDelta.x, mouseDelta.y);

        lastMousePosition = currentMousePosition;
    }
}

void Render::setCameraSwitchCallback(std::function<void()> callback)
{
    cameraSwitchCallback = std::move(callback);
}

void Render::processInput(GLFWwindow *pWindow)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->processKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->processKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera->processKeyboard(DOWN, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera->processKeyboard(ROLL_LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera->processKeyboard(ROLL_RIGHT, deltaTime);

    if (glfwGetKey(pWindow, GLFW_KEY_C) == GLFW_PRESS)
    {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastPressTimeC).count();
        if (duration > 200)
        {
            if (cameraSwitchCallback)
                cameraSwitchCallback();
            lastPressTimeC = now;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastPressTimeH).count();
        if (duration > 200)
        {
            hudOFF = !hudOFF;
            lastPressTimeH = now;
        }
    }
}

void Render::initQuad()
{
    std::vector<float> quadVertices = {
        //pos               //tex
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    std::vector<int> quadIndices = {
            0, 1, 3,
            1, 2, 3
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glGenBuffers(1, &quadEBO);

    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, quadVertices.size() * sizeof(float), static_cast<void*>(quadVertices.data()),
        GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, quadIndices.size() * sizeof(unsigned int),
        static_cast<void*>(quadIndices.data()), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void Render::initBloom()
{
    glGenFramebuffers(1, &lightFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);

    glGenTextures(2, colorBuffers);

    for (int i = 0; i < 2; ++i)
    {
        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCRWIDTH, SCRHEIGHT,
                     0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(
                GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D,
                colorBuffers[i], 0
        );
    }

    unsigned int attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, attachments);

    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongColorBuffers);

    for (int i = 0; i < 2; ++i)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongColorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCRWIDTH, SCRHEIGHT,
                     0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                               pingpongColorBuffers[i], 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
    }
}

void Render::initComputeShader()
{
    glGenTextures(1, &computeTexture);
    glBindTexture(GL_TEXTURE_2D, computeTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCRWIDTH, SCRHEIGHT, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Render::loadTextures(std::string &blackBody, std::vector<std::string> &cubeMap)
{
    blackBodyTexture = loadTexture(blackBody);
    cubeMapTexture = loadCubeMap(cubeMap);
}

void Render::setImGui(std::string fontPath, float fontSize)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    font = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), fontSize);
    ImGuiStyle &style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg].w = 0.5f;
}

void Render::draw(std::unique_ptr<ComputeShader> acomputeShader,
                  std::unique_ptr<Shader> BrightPassShader,
                  std::unique_ptr<Shader> BlurShader, std::unique_ptr<Shader> PostProcessShader)
{
    float lastTime = 0.0;
    float lastFrame = 0.0;
    int frame = 0;

    this->computeShader = std::move(acomputeShader);
    this->brightPassShader = std::move(BrightPassShader);
    this->blurShader = std::move(BlurShader);
    this->postProcessShader = std::move(PostProcessShader);

    while (!glfwWindowShouldClose(window))
    {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;

        frame++;
        if (currentTime - lastFrame >= 1.0)
        {
            std::cout << "FPS: " << frame << "\n";
            frame = 0;
            lastFrame += 1.0;
        }

        processInput(window);

        glm::mat4 view = camera->getViewMatrix();
        glm::vec3 cameraPos = camera->position;
        
        computeShader->use();
        computeShader->setVec2("resolution", 1920.0f, 1080.0f);
        computeShader->setFloat("time", (float) glfwGetTime());

        computeShader->setMat4("view", view);
        computeShader->setVec3("cameraPos", cameraPos);

        computeShader->setInt("integrationType", bh.integration);
        computeShader->setInt("disk", bh.disk);
        computeShader->setInt("accretionDiskOrbit", bh.accretionDiskOrbit);
        computeShader->setBool("dopplerShift", bh.dopplerEffect);
        computeShader->setBool("gravitationalRedShift", bh.gravitationalRedshift);
        computeShader->setBool("beaming", bh.beaming);
        computeShader->setBool("realisticTemperature", bh.realisticTemperature);
        computeShader->setFloat("accretionTemp", bh.accretionTemp);

        computeShader->setInt("blackbody", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, blackBodyTexture);
        computeShader->setInt("cubemap", 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
        glActiveTexture(GL_TEXTURE0);

        glBindImageTexture(0, computeTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);
        glDispatchCompute((unsigned int)(1920 / 16.0), (unsigned int)(1080 / 16.0), 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        //--------------------------------------------------------------------------------------
        glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
        brightPassShader->use();
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, computeTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        bool horizontal = true, firstIteration = true;
        unsigned int amount = 10;
        blurShader->use();
        for (unsigned int i = 0; i < amount; ++i)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
            blurShader->setInt("horizontal", horizontal);
            glBindTexture(GL_TEXTURE_2D, firstIteration ? colorBuffers[1] : pingpongColorBuffers[!horizontal]);

            glBindVertexArray(quadVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            horizontal = !horizontal;
            if (firstIteration)
                firstIteration = false;
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        postProcessShader->use();
        postProcessShader->setInt("toneMapping", rs.toneMapping);
        postProcessShader->setBool("bloom", rs.bloomEnabled);
        postProcessShader->setFloat("bloomStrength", rs.bloomStrength);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
        postProcessShader->setInt("screenTexture", 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, pingpongColorBuffers[!horizontal]);
        postProcessShader->setInt("bloomBlur", 1);
        glBindVertexArray(quadVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glActiveTexture(GL_TEXTURE0);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (hudOFF)
        {
            blackHoleWidget(bh, font);
            rendererWidget(rs, font);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        lastTime = currentTime;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}