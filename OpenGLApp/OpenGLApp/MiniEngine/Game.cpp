#include "Game.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdexcept>
#include "../Coin.h"

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
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME, NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    return window;
}

void Game::Update(float deltaTime)
{
    for (auto obj = activeObjects.begin(); obj != activeObjects.end(); obj++)
    {
        (*obj)->Update(deltaTime);
    }
}

void Game::Draw(Shader ourShader)
{
    // TEMPORARY DISPLAY OF CUBES
    for (auto obj = activeObjects.begin(); obj != activeObjects.end(); obj++)
    {
        (*obj)->Draw(ourShader);
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

bool Game::InstantiateGameObject(GameObject* newGameObject, Transform* spawnTransform, unsigned int texture)
{
    if (spawnTransform == nullptr)
    {
        throw std::runtime_error("Game: Trying to instantiate a new GameObject but transform is nullptr");
    }

    newGameObject->SetTextures(texture, texture);

    // copy content of transform into the new gameObject
	newGameObject->transform = *spawnTransform;
	activeObjects.push_back(newGameObject);

    // NOT TOO GOOD
    // it would be better if the ownership of the transform was given to the gameobject
    delete spawnTransform;
    // -------------------
	return true;
}

bool Game::InstantiateGameObject(GameObject* newGameObject, glm::vec3 position)
{
    Transform t = Transform(position, glm::vec3(.0f, .0f, .0f), glm::vec3(1.f, 1.f, 1.f));
    newGameObject->transform = t;
    activeObjects.push_back(newGameObject);
    return false;
}

void Game::DestroyGameObject(GameObject* gameObject)
{
    if (gameObject == nullptr)
    {
        throw std::runtime_error("Game: Trying to destroy a GameObject but pointer given is null");
    }
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
