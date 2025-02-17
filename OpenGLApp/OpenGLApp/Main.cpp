#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MiniEngine/camera.h"

#include <iostream>
#include <array>

#include "MiniEngine/Game.h"
#include "MiniEngine/SoundManager.h"
#include "MiniEngine/GameObject.h"

#define DELTA_TIME_MAX 0.1f

// settings
std::string gameName = "Space Defender";
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;
const float zNear = -20.0f;
const float zFar = 20.0f;
const float orthScale = 75.0f; // Parametro per zoomare e dezoomare gli oggetti con la camera ortografica


// camera
Camera camera(glm::vec3(0.0f, 0.0f, 15.0f));


// Game instance
Game& SpaceDefender = Game::Instance();

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    GLFWwindow* window = SpaceDefender.Setup(SCR_WIDTH, SCR_HEIGHT, gameName);

    // IMGUI init
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    SpaceDefender.Init();


    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    //-------------------------------------

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        if (deltaTime > DELTA_TIME_MAX)
            deltaTime = DELTA_TIME_MAX;

        lastFrame = currentFrame;

        // input
        // -----
        SpaceDefender.ProcessInput(deltaTime);
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SpaceDefender.Update(deltaTime);
        ImGui::Render();

        // Rendering
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
