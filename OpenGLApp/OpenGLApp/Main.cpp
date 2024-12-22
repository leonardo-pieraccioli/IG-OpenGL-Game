#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MiniEngine/assimp_API/shader.h"
#include "MiniEngine/camera.h"

#include <iostream>
#include <array>

#include "MiniEngine/Game.h"
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

    // build and compile our shader zprogram
    // ------------------------------------
    Shader shader("shader.vs", "shader.fs");

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    shader.use();

    SpaceDefender.Init();
    
    // CAMERA SETUP
    // ------------
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 15.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f));

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

        // activate shader
        shader.use();

        // Mi trovo la matrice ortografica per la camera
        glm::mat4 projection = glm::ortho(-((float) SCR_WIDTH / 2), (float)SCR_WIDTH / 2, -((float) SCR_HEIGHT / 2), (float)SCR_HEIGHT / 2, zNear, zFar);
        
        projection = glm::scale(projection, glm::vec3(orthScale, orthScale, 1.0f));
        shader.setMat4("projection", projection); 

        // camera/view transformation
        //glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("view", view);

        SpaceDefender.Update(deltaTime);
        SpaceDefender.Draw(shader);

        // glm::mat4 model_mat = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        //model_mat = glm::translate(model_mat, glm::vec3(0,0,0));
        //shader.setMat4("model", model_mat);
        //spaceShipModel.Draw(shader);

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
