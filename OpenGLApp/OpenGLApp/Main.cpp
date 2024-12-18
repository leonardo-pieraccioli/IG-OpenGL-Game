#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "MiniEngine/camera.h"

#include <iostream>
#include <array>

#include "MiniEngine/Game.h"
#include "MiniEngine/GameObject.h"
#include "Planet.h"
#include "Ship.h"
#include "Player.h"
#include "Coin.h"
#include "MiniEngine/ResourceLoader.h"

// settings
std::string gameName = "Space Defender";
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
const float zNear = -20.0f;
const float zFar = 20.0f;
const float orthScale = 50.0f; // Parametro per zoomare e dezoomare gli oggetti con la camera ortografica
GLFWwindow* window;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 15.0f));

// Game instance
Game& SpaceDefender = Game::Instance();

// player (da spostare)
Player* player = new Player();

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// TRANSFER INTO THE ENGINE
void ProcessInput(float deltaTime);

int main()
{
    window = SpaceDefender.Setup(SCR_WIDTH, SCR_HEIGHT, gameName);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("shader.vs", "shader.fs");

    // --------------------------------------
    // TEMPORARY SECTION: CUBE INITIALIZATION

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // --------------------------

    // load and create textures 
    // -------------------------
    //unsigned int text_container = LoadTexture("container.jpg", false);
    //unsigned int text_awesomeFace = LoadTexture("awesomeface.png", true);
    unsigned int text_coin = LoadTexture("coin_black.png", true);
    //unsigned int text_coinBlack = LoadTexture("coin_black.png", true);
    unsigned int text_spaceship = LoadTexture("spaceship.png", true);
    unsigned int text_planet = LoadTexture("planet.png", false);
    //unsigned int text_spaceshipBlack = LoadTexture("spaceship_black.png", true);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);


    // Example data structure of all game objects
    // Esempio temporaneo per dimostrare funzionamento del polimorfismo

    glm::vec3 planetScale = glm::vec3(3.0f, 3.0f, 3.0f);
    SpaceDefender.InstantiateGameObject(new Planet(), new Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.f, 0.f, 0.f), planetScale), text_planet);
    SpaceDefender.InstantiateGameObject(player, new Transform(), text_spaceship);
    //SpaceDefender.InstantiateGameObject(new Ship(), new Transform(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.f, 0.f, 0.f), shipScale));
    
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
        ProcessInput(deltaTime);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader
        ourShader.use();

        // Mi trovo la matrice ortografica per la camera
        glm::mat4 projection = glm::ortho(-((float) SCR_WIDTH / 2), (float)SCR_WIDTH / 2, -((float) SCR_HEIGHT / 2), (float)SCR_HEIGHT / 2, zNear, zFar);
        
        projection = glm::scale(projection, glm::vec3(orthScale, orthScale, 1.0f));
        ourShader.setMat4("projection", projection); 

        // camera/view transformation
        //glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);
        
        // render boxes
        glBindVertexArray(VAO);

        // generazione monete
        Coin::generateCoins(deltaTime, SpaceDefender, text_coin);

        SpaceDefender.Update(deltaTime);
        SpaceDefender.Draw(ourShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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
        player->moveHip(1, deltaTime);                  // Da sostituire
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        player->moveHip(0, deltaTime);

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
        SpaceDefender.CheckCoins(glm::vec3(xworld, yworld, 0.0));
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