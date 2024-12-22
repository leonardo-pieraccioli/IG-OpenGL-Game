#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "MiniEngine/camera.h"

#include <iostream>
#include <array>

#include "MiniEngine/Game.h"
#include "MiniEngine/ResourceLoader.h"
#include "MiniEngine/SoundManager.h"


// settings
std::string gameName = "Space Defender";
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
GLFWwindow* window;


// camera
Camera camera(glm::vec3(0.0f, 0.0f, 15.0f));


// Game instance
Game& SpaceDefender = Game::Instance();


// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


// TRANSFER INTO THE ENGINE
void ProcessInput(float deltaTime);


// AUDIO


int main()
{
    window = SpaceDefender.Setup(SCR_WIDTH, SCR_HEIGHT, gameName);

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
        ProcessInput(deltaTime);

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

    // optional: de-allocate all resources once they've outlived their purpose:    !!!!!!!!!!!!!!!!! DA METTERE NEL DISTRUTTORE DI GAME
    // ------------------------------------------------------------------------
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void ProcessInput(float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Game::Instance().getPlayer()->moveHip(1, deltaTime);                  // Da sostituire
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Game::Instance().getPlayer()->moveHip(0, deltaTime);

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        // TEMPORARY: 12.5 and 6.5 are the approximate coordinates of the limits
        //            of what we can see with the camera.
        //            The function should map mouse coordinates more precisely
        // ---------------------------------------------------------------------
        // xpos : 1280 = xworld : 12.5 -> xworld = xpos * 12.5 / 1280
        double xworld = (xpos * 26 / SpaceDefender.SCREEN_WIDTH) - 13;
        double yworld = -((ypos * 13 / SpaceDefender.SCREEN_HEIGHT) - 6.5);
        // ---------------------------------------------------------------------

        // std::cout << "Mouse clicked in " << xworld << ":" << yworld << std::endl;
        SpaceDefender.CheckCoins(glm::vec3(xworld, yworld, 0.0), Game::Instance().getPlayer());
    }

}

/*                                                                      Esperimento
std::pair<float, float> ScreenCoordToWorldCoord() {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    glm::mat4 projection = glm::ortho(-((float)SCR_WIDTH / 2), (float)SCR_WIDTH / 2, -((float)SCR_HEIGHT / 2), (float)SCR_HEIGHT / 2, zNear, zFar);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    const float ndc_x = (float)(2 * xpos - SCR_WIDTH) / (float)SCR_WIDTH; 
    const float ndc_y = (float)(SCR_HEIGHT - 2 * ypos) / (float)SCR_HEIGHT;
    glm::vec4 point = glm::inverse(projection) * glm::vec4(ndc_x, ndc_y, -1.f, 1.f);//inv_P = inverse projection matrix
    point = point / point.w;
    glm::vec4 world = glm::inverse(view) * point; //inv_V = inverse view matrix
}
*/