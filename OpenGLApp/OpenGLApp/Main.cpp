#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MiniEngine/camera.h"

#include <iostream>
#include <array>

#include "MiniEngine/Game.h"
#include "MiniEngine/SoundManager.h"
#include "MiniEngine/GameObject.h"


// settings
std::string gameName = "Space Defender";
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
const float zNear = -20.0f;
const float zFar = 20.0f;
const float orthScale = 50.0f; // Parametro per zoomare e dezoomare gli oggetti con la camera ortografica


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

    SpaceDefender.Init();
    
    // Background music
    SoundManager::Instance().setup();
    SoundManager::Instance().playSound("Assets/Sounds/star_striker.mp3", true);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        SpaceDefender.ProcessInput(deltaTime);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SpaceDefender.Update(deltaTime);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
