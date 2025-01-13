#include "Game.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdexcept>

#include "../Coin.h"
#include "../Planet.h"
#include "../Enemy.h"

GLFWwindow* gameWindow;
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 15.0f);

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

    // Default Shader
    shader = ResourceManager::LoadShader("shader.vs", "shader.fs", nullptr, "DefaultShader");
    //lightShader = ResourceManager::LoadShader("shader_light.vs", "shader_light.fs", nullptr, "LightShader");
    lightingShader = ResourceManager::LoadShader("shaderLighting.vs", "shaderLighting.fs", nullptr, "LightingShader");

    // TEMPORANEO, UNA SCHIFEZZA ASSOLUTA MA PER IL MOMENTO SEMBRA ANDARE ------------------------------------------------------------------------------ //

    lightingShader.Use();
    lightingShader.SetVector3f("light.position", lightPos);
    lightingShader.SetVector3f("viewPos", cameraPosition);
    // light properties
    glm::vec3 lightColor;
    lightColor.x = 1.0f;
    lightColor.y = 1.0f;
    lightColor.z = 1.0f;
    glm::vec3 diffuseColor = lightColor * glm::vec3(0.7f); // decrease the influence
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.4f); // low influence
    lightingShader.SetVector3f("light.ambient", ambientColor);
    lightingShader.SetVector3f("light.diffuse", diffuseColor);
    lightingShader.SetVector3f("light.specular", 1.0f, 1.0f, 1.0f);

    // material properties
    lightingShader.SetVector3f("material.ambient", 1.0f, 1.0f, 1.0f);
    lightingShader.SetVector3f("material.diffuse", 1.0f, 1.0f, 1.0f);
    lightingShader.SetVector3f("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
    lightingShader.SetFloat("material.shininess", 10.0f);

    // --------------------------------------------------------------------------------------------------------------------------------------------------- //
  
    // CAMERA SETUP
    // ------------
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 15.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

    TextManager::Instance().InitManager(SCR_WIDTH, SCR_HEIGHT);
    TextManager::Instance().LoadFont("resources/fonts/Space Age/space age.ttf", "Space Age");
    TimerManager::CreateTimer(300.0f, true, "Round Timer", true, this);
    //TextManager::Instance().LoadFont("resources/fonts/Antonio/static/Antonio-Bold.ttf", "Antonio-Bold");

    return gameWindow;
}

void Game::Init()
{
    player = new Player();
    planet = new Planet();
    
    InstantiateGameObject(planet, new Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.25f, 1.25f, 1.25f)));
    InstantiateGameObject(player, new Transform());
	
    auto enemy = new Enemy();
    InstantiateGameObject(enemy, new Transform(glm::vec3(-15.f, 15.0f, 0.0f), glm::vec3(0.f, 0.f, 315.f), glm::vec3(.25f, .25f, .25f)));
	auto enemy2 = new Enemy();
	InstantiateGameObject(enemy2, new Transform(glm::vec3(15.f, 15.0f, 0.0f), glm::vec3(0.f, 0.f, 225.f), glm::vec3(.25f, .25f, .25f)));
	auto enemy3 = new Enemy();
	InstantiateGameObject(enemy3, new Transform(glm::vec3(15.f, -15.0f, 0.0f), glm::vec3(0.f, 0.f, 135.f), glm::vec3(.25f, .25f, .25f)));
	auto enemy4 = new Enemy();
	InstantiateGameObject(enemy4, new Transform(glm::vec3(-15.f, -15.0f, 0.0f), glm::vec3(0.f, 0.f, 45.f), glm::vec3(.25f, .25f, .25f)));

    // SpaceDefender.InstantiateGameObject(new Ship(), new Transform(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.f, 0.f, 0.f), shipScale));
}

void Game::Update(float deltaTime)
{
    
    // generazione monete
    Coin::generateCoins(deltaTime);

    for (auto obj = activeObjects.begin(); obj != activeObjects.end(); )
    {
        (*obj)->Update(deltaTime);
        if (!(*obj)->isActive)
        {
            obj = activeObjects.erase(obj);
        }
        else
        {
            obj++;
        }
    }  
    Draw(lightingShader);

    TimerManager::updateTimers(deltaTime);

        std::string roundTimeText = TimerManager::GetTimer("Round Timer").getHH_MM_SS_MS();
    TextManager::Instance().RenderText(roundTimeText, 430, 680, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f), "Space Age");

    std::string scoreText = "Score: " + std::to_string(player->getMoney());
    TextManager::Instance().RenderText(scoreText, 0, 0, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f), "Space Age");
}

void Game::Draw(Shader shader)
{
    shader.Use();

    // Mi trovo la matrice ortografica per la camera
    projection = glm::ortho(-((float)SCR_WIDTH / 2), (float)SCR_WIDTH / 2, -((float)SCR_HEIGHT / 2), (float)SCR_HEIGHT / 2, zNear, zFar);
    projection = glm::scale(projection, glm::vec3(orthScale, orthScale, 1.0f));
    shader.SetMatrix4("projection", projection);

    // camera/view transformation
    //glm::mat4 view = camera.GetViewMatrix();
    shader.SetMatrix4("view", view);

    // render boxes
    glBindVertexArray(VAO);

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
    if (glfwGetKey(gameWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
              player->shootWithShips();

    if (glfwGetMouseButton(gameWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        double mouseX, mouseY;
        glfwGetCursorPos(gameWindow, &mouseX, &mouseY);

        // Get window size
        int width, height;
        glfwGetWindowSize(gameWindow, &width, &height);

        // Convert mouse position to normalized device coordinates (NDC)
        float x = (2.0f * mouseX) / width - 1.0f;
        float y = 1.0f - (2.0f * mouseY) / height; // Invert y to match OpenGL's coordinate system
        float z = 0.0f; // z is 0 in NDC for a 2D orthographic projection

        // Convert NDC to world coordinates
        glm::vec4 ndcPos(x, y, z, 1.0f);

        // Compute the inverse of the view-projection matrix
        glm::mat4 invViewProj = glm::inverse(projection * view);

        // Transform NDC to world coordinates
        glm::vec4 worldPos = invViewProj * ndcPos;

        // Normalize if w is not 1 (perspective divide)
        if (worldPos.w != 0.0f) {
            worldPos /= worldPos.w;
        }

        auto worldCoordinates = glm::vec3(worldPos); // x, y, z in world coordinates

        CheckCoins(glm::vec3(worldCoordinates.x, worldCoordinates.y, 0.0));
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

    newGameObject->isActive = true;
	newGameObject->transform = *spawnTransform;
	activeObjects.push_back(newGameObject);
	return true;
}

void Game::DestroyGameObject(GameObject* gameObject)
{
    gameObject->isActive = false;
}

void Game::setPlayer(Player* player)
{
    this->player = player;
}

Player* Game::getPlayer()
{
    return player;
}

// TEMP: colliders should have a more general behavior and polling every game object
//       at every click of the mouse is a waste of performance
void Game::CheckCoins(glm::vec3 coinPosition)
{
    
    for (auto obj = activeObjects.begin(); obj != activeObjects.end(); obj++)
    {
        Coin* coin = dynamic_cast<Coin*>(*obj);
        if (coin) {
            if (coin->doesCoinOverlap(coinPosition)) {
                player->addMoney(coin->getMoney());
                SoundManager::Instance().playSound("Assets/Sounds/coin_pickup.mp3", false);
                DestroyGameObject(coin);
                break;
            }
        }
    }
}

GameObject* Game::CheckCollision(GameObject& caller, glm::vec3 position, glm::vec3 scale)
{
    auto upperLeft = std::pair<float, float>(position.x - scale.x/2, position.y - scale.y/2);
    auto lowerRight = std::pair<float, float>(position.x + scale.x / 2, position.y + scale.y / 2);

    for (auto obj = activeObjects.begin(); obj != activeObjects.end(); obj++)
    {
		if ((*obj) == &caller)
			continue;

		auto otherUpperLeft = std::pair<float, float>((*obj)->transform.getPosition().x - (*obj)->transform.getScale().x, (*obj)->transform.getPosition().y - (*obj)->transform.getScale().y);
		auto otherLowerRight = std::pair<float, float>((*obj)->transform.getPosition().x + (*obj)->transform.getScale().x, (*obj)->transform.getPosition().y + (*obj)->transform.getScale().y);

		if (upperLeft.first < otherLowerRight.first && lowerRight.first > otherUpperLeft.first &&
			upperLeft.second < otherLowerRight.second && lowerRight.second > otherUpperLeft.second)
		{
            std::cout << "Collision detected between two objects" << std::endl;
			return *obj;
		}
    }
    
    return nullptr;
}

void Game::getNotified(std::string timerName, bool isCallbackEnabled)
{
    if(timerName == "Round Timer")
        cout << "Il tempo per questo round è finito!\n";
}
