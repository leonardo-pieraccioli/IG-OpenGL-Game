#include "Game.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdexcept>

// ---------------
// INPUT CALLBACKS
// ---------------

// mouse callback
float lastX;
float lastY;
bool firstMouse = true;

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    // camera.ProcessMouseMovement(xoffset, yoffset);
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, (*obj)->transform.getPosition());
        model = glm::rotate(model, glm::radians((*obj)->transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch
        model = glm::rotate(model, glm::radians((*obj)->transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Yaw
        model = glm::rotate(model, glm::radians((*obj)->transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Roll

        ourShader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
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

bool Game::InstantiateGameObject(GameObject* newGameObject, Transform* spawnTransform)
{
    if (spawnTransform == nullptr)
    {
        throw std::runtime_error("Game: Trying to instantiate a new GameObject but transform is nullptr");
    }

	newGameObject->transform = *spawnTransform;
	activeObjects.push_back(newGameObject);

	return true;
}

void Game::DestroyGameObject(GameObject& gameObject)
{
	//ActiveObjects.remove(gameObject);
}
