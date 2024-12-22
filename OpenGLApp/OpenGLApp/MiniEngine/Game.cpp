#include "Game.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdexcept>

#include "../Coin.h"

GLFWwindow* gameWindow;

// ---------------
// INPUT CALLBACKS
// ---------------

// mouse callback
float lastX;
float lastY;
bool mouseInputPressedOnce = false;

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    Game::Instance().SCREEN_HEIGHT = height;
    Game::Instance().SCREEN_WIDTH = width;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);


}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// ---------------
// SETUP FUNCTIONS
// ---------------

GLFWwindow* Game::Setup(int screenWidth, int screenHeight, std::string gameName)
{
    SCREEN_WIDTH = screenWidth;
    SCREEN_HEIGHT = screenHeight;
    GAME_NAME = gameName.c_str();
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    gameWindow = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME, NULL, NULL);
    if (gameWindow == NULL)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(gameWindow);
    glfwSetFramebufferSizeCallback(gameWindow, framebuffer_size_callback);
    glfwSetCursorPosCallback(gameWindow, mouse_callback);
    glfwSetScrollCallback(gameWindow, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(gameWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    return gameWindow;
}


void Game::Init()
{
    player = new Player();
    planet = new Planet();
    InstantiateGameObject(planet, new Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.25f, 1.25f, 1.25f)));
    InstantiateGameObject(player, new Transform());
    // SpaceDefender.InstantiateGameObject(new Ship(), new Transform(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.f, 0.f, 0.f), shipScale));
}

void Game::Update(float deltaTime)
{
    // generazione monete
    Coin::generateCoins(deltaTime);

    for (auto obj = activeObjects.begin(); obj != activeObjects.end(); obj++)
    {
        (*obj)->Update(deltaTime);
    }
}

void Game::Draw(Shader shader)
{
    // TEMPORARY DISPLAY OF CUBES
    for (auto obj = activeObjects.begin(); obj != activeObjects.end(); obj++)
    {
        (*obj)->Draw(shader);
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Game::ProcessInput(float deltaTime)
{
    if (glfwGetKey(gameWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(gameWindow, true);

    if (glfwGetKey(gameWindow, GLFW_KEY_A) == GLFW_PRESS)
        player->moveHip(1, deltaTime);                  // Da sostituire
    if (glfwGetKey(gameWindow, GLFW_KEY_D) == GLFW_PRESS)
        player->moveHip(0, deltaTime);

    double xpos, ypos;
    glfwGetCursorPos(gameWindow, &xpos, &ypos);

    if (glfwGetMouseButton(gameWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        // TEMPORARY: 12.5 and 6.5 are the approximate coordinates of the limits
        //            of what we can see with the camera.
        //            The function should map mouse coordinates more precisely
        // ---------------------------------------------------------------------
        // xpos : 1280 = xworld : 12.5 -> xworld = xpos * 12.5 / 1280
        double xworld = (xpos * 26 / SCREEN_WIDTH) - 13;
        double yworld = -((ypos * 13 / SCREEN_HEIGHT) - 6.5);
        // ---------------------------------------------------------------------

        // std::cout << "Mouse clicked in " << xworld << ":" << yworld << std::endl;
        CheckCoins(glm::vec3(xworld, yworld, 0.0));
    }

}

Game& Game::Instance()
{
	static auto&& instance = Game();
	return (instance);
}

// ----------------------------------
// OBJECT DATA STRUCTURE MANIPULATION
// ----------------------------------

bool Game::InstantiateGameObject(GameObject* newGameObject, Transform* spawnTransform)//, Model model)
{
    if (spawnTransform == nullptr)
    {
        throw std::runtime_error("Game: Trying to instantiate a new GameObject but transform is nullptr");
    }

	newGameObject->transform = *spawnTransform;
    //newGameObject->objectModel = model;
	activeObjects.push_back(newGameObject);
	return true;
}

void Game::DestroyGameObject(GameObject* gameObject)
{
	activeObjects.remove(gameObject);
}

// TEMP: colliders should have a more general behavior and polling every game object
//       at every click of the mouse is a waste of performance
void Game::CheckCoins(glm::vec3 coinPosition)
{
    
    for (auto obj = activeObjects.begin(); obj != activeObjects.end(); obj++)
    {
        Coin* coin = dynamic_cast<Coin*>(*obj);
        if (coin) {
            if (coin->shouldDestroy(coinPosition)) // coordinate da sostituire
                break;
        }
    }
}
