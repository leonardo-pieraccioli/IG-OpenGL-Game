#include "Game.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdexcept>

#include "assimp_API/stb_image.h"

#include "../Coin.h"
#include "../PowerUpNerf.h"
#include "../Planet.h"
#include "../Enemy.h"
#include "../UI.h"

constexpr auto TIMER_DURATION = 30.f;

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

    // FULLSCREEN
    // La risoluzione va settata come quella del monitor, altrimenti fa i resize sbagliati
    // Questo probabilmente si può fare prendendosi la variabile monitor tramite glfwGetPrimaryMonitor
    // In fullscreen si nota una cornice leggera bianca, probabilmente dovuta alle window di ImGui
    // Probabilmente si risolve inserendo qualche flag
    // La cosa migliore sarebbe fare in modo che il fullscreen potesse essere messo e tolto
    // Se si prova a fare alt+tab mentre si è in fullscreen non riesce più a tornare dentro la finestra.
    // Probabilmente c'è un cambio di contesto che non riusciamo a gestire
    //
    // TLDR: è decisamente più bello in fullscreen, ma probabilmente richiede più lavoro di quanto 
    //       riusciamo a metterci. Sarebbe bello sbloccarlo in futuro.
    //gameWindow = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME, glfwGetPrimaryMonitor(), nullptr);
    //--------------------------------------------------------------------------------------------------------

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
	simpleShader = SimpleShader("simple_shader.vs", "simple_shader.fs");

    // TEMPORANEO, UNA SCHIFEZZA ASSOLUTA MA PER IL MOMENTO SEMBRA ANDARE ------------------------------------------------------------------------------ //

    lightingShader.Use();
    lightingShader.SetVector3f("light.position", lightPos);
    lightingShader.SetVector3f("viewPos", cameraPosition);
    // light properties
    glm::vec3 lightColor;
    lightColor.x = 1.0f;
    lightColor.y = 1.0f;
    lightColor.z = 1.0f;
    glm::vec3 diffuseColor = lightColor * glm::vec3(0.8f); // decrease the influence
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.4f); // low influence
    lightingShader.SetVector3f("light.ambient", ambientColor);
    lightingShader.SetVector3f("light.diffuse", diffuseColor);
    lightingShader.SetVector3f("light.specular", 1.0f, 1.0f, 1.0f);

    // material properties
    lightingShader.SetVector3f("material.ambient", 1.0f, 1.0f, 1.0f);
    lightingShader.SetVector3f("material.diffuse", 1.0f, 1.0f, 1.0f);
    lightingShader.SetVector3f("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
    lightingShader.SetFloat("material.shininess", 10.0f);
    lightingShader.SetInteger("shouldActivateHalftoning", 0);

    // --------------------------------------------------------------------------------------------------------------------------------------------------- //

    // CAMERA SETUP
    // ------------
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 15.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

    TextManager::Instance().InitManager(SCR_WIDTH, SCR_HEIGHT);
    TextManager::Instance().LoadFont("resources/fonts/Space Age/space age.ttf", "Space Age");
    //TextManager::Instance().LoadFont("resources/fonts/Antonio/static/Antonio-Bold.ttf", "Antonio-Bold");

    // Music
    SoundManager::Instance().setup();
    ost = SoundManager::Instance().playSoundWithRetP("Assets/Sounds/star_striker.mp3", true);

    gameState = GameState::Menu;

    return gameWindow;
}

void Game::Init()
{
    roundTimer = TimerManager::CreateTimer(TIMER_DURATION, true, "Round Timer", true, this);
    bonusTimer = TimerManager::CreateTimer(modifierTimeAmount, false, "Bonus Timer", true, this);
    malusTimer = TimerManager::CreateTimer(modifierTimeAmount, false, "Malus Timer", true, this);

    player = new Player();
    planet = new Planet();
    Enemy::Init(Model("Assets/Models/enemy1.obj"));

    sky = new Sky();
    InstantiateGameObject(planet, new Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.5f, 1.5f, 1.5f)));
    InstantiateGameObject(player, new Transform());
    
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    
    font_SA_menu = io.Fonts->AddFontFromFileTTF("resources/fonts/Space Age/space age.ttf", 120.f);
    font_SA_large = io.Fonts->AddFontFromFileTTF("resources/fonts/Space Age/space age.ttf", 60.f);
    font_SA_medium = io.Fonts->AddFontFromFileTTF("resources/fonts/Space Age/space age.ttf", 40.f);
    font_SA_small = io.Fonts->AddFontFromFileTTF("resources/fonts/Space Age/space age.ttf", 25.f);
}

void Game::Update(float deltaTime)
{
    switch (gameState) {
        case GameState::Play:
        {
            // generazione nemici
            Enemy::generateEnemies(deltaTime);

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

            UIPlay();

            break;
        }
        case GameState::Pause:
        {
            Draw(lightingShader);
            UIPause();
            
            break;
        }
        case GameState::Menu:
        {
			Draw(lightingShader);
			drawMenuModel(lightingShader);
            UIMenu();

            break;
        }
        case GameState::Controls:
        {
            // draw sky
            simpleShader.use();
            simpleShader.setMat4("projection", projection);
            simpleShader.setMat4("view", view);
            simpleShader.setMat4("model", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -4)), glm::vec3(28.f, 16.5f, 0.0f)));
            sky->Draw();

            UIControls();
            break;
        }
        case GameState::GameOver:
        {
            Draw(lightingShader);
            UIGameOver();
            break;
        }
        case GameState::Shop:
        {
            Draw(lightingShader);

            UIShop();
               
            break;
        }
        case GameState::Quit:
        {
            glfwSetWindowShouldClose(gameWindow, true);
            break;
        }
    }
}

void Game::Draw(Shader shader)
{
    projection = glm::ortho(-((float)SCR_WIDTH / 2), (float)SCR_WIDTH / 2, -((float)SCR_HEIGHT / 2), (float)SCR_HEIGHT / 2, zNear, zFar);
    projection = glm::scale(projection, glm::vec3(orthScale, orthScale, 1.0f));

    // all other objects
    shader.Use();
    shader.SetMatrix4("projection", projection);
    shader.SetMatrix4("view", view);
    glBindVertexArray(VAO);
    for (auto obj = activeObjects.begin(); obj != activeObjects.end(); obj++)
        (*obj)->Draw(shader);

    // draw sky
    simpleShader.use();
    simpleShader.setMat4("projection", projection);
    simpleShader.setMat4("view", view);
    simpleShader.setMat4("model", glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -4)), glm::vec3(28.f, 16.5f, 0.0f)));
    sky->Draw();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Game::ProcessInput(float deltaTime)
{
    if (gameState == GameState::Play){
        if (glfwGetKey(gameWindow, GLFW_KEY_A) == GLFW_PRESS) {
            aHeldDown = true;
            player->moveHip(1, deltaTime);
            player->setPitchRotationValue(-1);
        }
        if (glfwGetKey(gameWindow, GLFW_KEY_D) == GLFW_PRESS) {
            dHeldDown = true;
            player->moveHip(0, deltaTime);
            player->setPitchRotationValue(1);
        }
        if (aHeldDown && glfwGetKey(gameWindow, GLFW_KEY_A) == GLFW_RELEASE) {
            aHeldDown = false;
            player->setPitchRotationValue(0);
        }
        if (dHeldDown && glfwGetKey(gameWindow, GLFW_KEY_D) == GLFW_RELEASE) {
            dHeldDown = false;
            player->setPitchRotationValue(0);
        }
        if (glfwGetKey(gameWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
            player->shootWithShips();
        if (glfwGetKey(gameWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            pHeldDown = true;
        if (pHeldDown && glfwGetKey(gameWindow, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
            pHeldDown = false;
           gameState = GameState::Pause;
        }
        /*if (glfwGetKey(gameWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
            player->shootWithShips();
        if (glfwGetKey(gameWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
            player->shootWithShips();*/

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
            CheckCollectables(glm::vec3(worldCoordinates.x, worldCoordinates.y, 0.0));
        }
    }
    else if (gameState == GameState::Pause) {
        if (glfwGetKey(gameWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            pHeldDown = true;
        if (pHeldDown && glfwGetKey(gameWindow, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
            pHeldDown = false;
            gameState = GameState::Play;
        }
    }
    else if (gameState == GameState::Menu) {
        if (glfwGetKey(gameWindow, GLFW_KEY_ENTER) == GLFW_PRESS)
            gameState = GameState::Play;
    }
    else if (gameState == GameState::Shop) {
        if (glfwGetKey(gameWindow, GLFW_KEY_ENTER) == GLFW_PRESS)
            gameState = GameState::Play;
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

// ----------
// GAME STATE
// ----------
void Game::ChangeGameState(GameState newGameState)
{
    // make exit transitions
    switch (gameState)
    {
    case GameState::Play:

        break;

    case GameState::Shop:

        break;

    case GameState::Pause:

        break;
    }
    gameState = newGameState;
}

void Game::CheckCollectables(glm::vec3 collectablePosition)
{
    for (auto obj = activeObjects.begin(); obj != activeObjects.end(); obj++)
    {
        Collectables* collectable = dynamic_cast<Collectables*>(*obj);
        if (collectable && collectable->doesCollectableOverlap(collectablePosition)) {
            if (collectable->CompareTag("Coin")) {
                Coin* coin = dynamic_cast<Coin*>(collectable);
                player->addMoney(coin->getMoney());
				player->addScore(50);
                SoundManager::Instance().playSound("Assets/Sounds/coin_pickup.mp3", false);
                DestroyGameObject(coin);
                break;
            }
            else if (collectable->CompareTag("PowerUp")) {
				auto powerUp = dynamic_cast<PowerUpNerf*>(collectable);
                // bonusTimer->resetTimer(true);
				planet->health.Heal(2+round);
                player->addScore(20);
				// Enemy::Nerf(true, powerUp->getModifiedShootingRate(), powerUp->getModifiedMovementRate());
                SoundManager::Instance().playSound("Assets/Sounds/bonus.mp3", false);
                DestroyGameObject(collectable);
                break;
            }
            else if (collectable->CompareTag("Nerf")) {
				std::cout << "Nerf" << std::endl;
				auto nerf = dynamic_cast<PowerUpNerf*>(collectable);
				player->Nerf(true, nerf->getModifiedMovementRate(), nerf->getModifiedShootingRate());
                malusTimer->resetTimer(true);
                SoundManager::Instance().playSound("Assets/Sounds/malus.mp3", false);
                lightingShader.Use();
                lightingShader.SetInteger("shouldActivateHalftoning", 1);
                SoundManager::Instance().changePitch(ost, 0.85f);
                DestroyGameObject(collectable);
                break;
            }
        }
    }
}

GameObject* Game::CheckCollision(GameObject& caller, glm::vec3 position, glm::vec3 scale)
{
	auto thisRadius = caller.transform.collisionRadius;
    auto thisCenter = position;
    
    for (auto obj = activeObjects.begin(); obj != activeObjects.end(); obj++)
    {
        if ((*obj)->CompareTag("Sky"))
        {
            continue;
        }

        if ( (*obj)->CompareTag("Player"))
        {
            GameObject* hit = player->CheckShipCollision(position, thisRadius);
            if (hit != nullptr)
            {
                return hit;
            }
        }
        else if ((*obj) != &caller)
        {
            auto otherRadius = (*obj)->transform.collisionRadius;
            auto otherCenter = (*obj)->transform.getPosition();

            if (glm::distance(thisCenter, otherCenter) < thisRadius + otherRadius)
            {
                return *obj;
            }
        }
    }
    
    return nullptr;
}

void Game::getNotified(std::string timerName, bool isCallbackEnabled)
{
    if(timerName == "Round Timer") {
        gameState = GameState::Shop;
        roundTimer->resetTimer(true);
        round++;
    }
    if (timerName == "Bonus Timer") {
        //Enemy::Nerf(false);
    }
    if (timerName == "Malus Timer") {
		this->player->Nerf(false);
		lightingShader.Use();
        lightingShader.SetInteger("shouldActivateHalftoning", 0);
        SoundManager::Instance().changePitch(ost, 1.0f);
    }
}

void Game::resetGame()
{
    SoundManager::Instance().stopAllSounds();
    UpgradeManager::Instance().reset();
    TimerManager::DestroyTimers();

    roundTimer = TimerManager::CreateTimer(TIMER_DURATION, true, "Round Timer", true, this);
    bonusTimer = TimerManager::CreateTimer(modifierTimeAmount, false, "Bonus Timer", true, this);
    malusTimer = TimerManager::CreateTimer(modifierTimeAmount, false, "Malus Timer", true, this);

    player->resetPlayer();
    planet->resetPlanet();
	Enemy::setCurrentEnemyCount(0);

    round = 1;
    for (auto obj = activeObjects.begin(); obj != activeObjects.end(); )
    {
        if (dynamic_cast<Enemy*>(*obj) || dynamic_cast<Collectables*>(*obj) || dynamic_cast<Projectile*>(*obj))
        {
            obj = activeObjects.erase(obj);
        }
        else
        {
            obj++;
        }
    }
    ost = SoundManager::Instance().playSoundWithRetP("Assets/Sounds/star_striker.mp3", true);
    lightingShader.SetInteger("shouldActivateHalftoning", 0);
}

void Game::drawMenuModel(Shader shader)
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
    glm::mat4 model_mat = glm::mat4(1.0f);  // make sure to initialize matrix to identity matrix first
    model_mat = glm::translate(model_mat, glm::vec3(3, 0, 3.5));
    model_mat = glm::rotate(model_mat, -45.f, glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch
    model_mat = glm::rotate(model_mat, 0.f, glm::vec3(0.0f, 1.0f, 0.0f));   // Yaw
    model_mat = glm::rotate(model_mat, 15.f, glm::vec3(0.0f, 0.0f, 1.0f));  // Roll
    model_mat = glm::scale(model_mat, glm::vec3(2.f, 2.f, 2.f));
    shader.SetMatrix4("model", model_mat);
	player->objectModel.Draw(shader);
}

int Game::getRound()
{
    return round;
}

void Game::upgrade(UpgradeIndex upgradeIndex)
{
    auto& ships = player->shipArray;

    switch (upgradeIndex) {
        case ShipsNumber:
            player->addScore(500);
            player->upgrade(upgradeIndex);
            break;
        case ShootingRate:
            player->addScore(75);
            player->upgrade(upgradeIndex);
            break;
        case BulletsNumber:
			player->addScore(500);
			player->upgrade(upgradeIndex);
			break;
        case MaxShipsHealth:
			player->addScore(100);
			player->upgrade(upgradeIndex);
			break;
        case Damage:
			player->addScore(100);
            for (int i = 0; i < player->shipArray.size(); i++) {
                ships[i]->upgrade(upgradeIndex);
            }
            break;
        case PlanetHealth:
			player->addScore(75);
            planet->upgrade(upgradeIndex);
            break;
		case ShipsSpeed:
			player->addScore(100);
			player->upgrade(upgradeIndex);
			break;
        default:
            cout << "Errore, upgradeIndex fuori dal range di potenziamenti disponibili" << endl;
    }
}
