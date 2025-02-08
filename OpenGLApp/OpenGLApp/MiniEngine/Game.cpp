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

    // --------------------------------------------------------------------------------------------------------------------------------------------------- //
  
    // CAMERA SETUP
    // ------------
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 15.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

    TextManager::Instance().InitManager(SCR_WIDTH, SCR_HEIGHT);
    TextManager::Instance().LoadFont("resources/fonts/Space Age/space age.ttf", "Space Age");
    //TextManager::Instance().LoadFont("resources/fonts/Antonio/static/Antonio-Bold.ttf", "Antonio-Bold");

    gameState = GameState::Menu;

    return gameWindow;
}

void Game::Init()
{
    roundTimer = TimerManager::CreateTimer(300.0f, true, "Round Timer", true, this);

    player = new Player();
    planet = new Planet();
    Enemy::Init(Model("Assets/Models/enemy1.obj"));

    GameObject* sky = new GameObject("Sky");
    sky->objectModel = Model("Assets/Models/sky.obj");

    InstantiateGameObject(sky, new Transform(glm::vec3(0.0f, 0.0f, -4.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(8.5f, 8.5f, 6.0f)));
    InstantiateGameObject(planet, new Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.5f, 1.5f, 1.5f)));
    InstantiateGameObject(player, new Transform());
    
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    
    font_SA_menu = io.Fonts->AddFontFromFileTTF("resources/fonts/Space Age/space age.ttf", 120.f);
    font_SA_large = io.Fonts->AddFontFromFileTTF("resources/fonts/Space Age/space age.ttf", 60.f);
    font_SA_medium = io.Fonts->AddFontFromFileTTF("resources/fonts/Space Age/space age.ttf", 40.f);
    font_SA_small = io.Fonts->AddFontFromFileTTF("resources/fonts/Space Age/space age.ttf", 30.f);
}

// -----
// IMGUI
void TextCentered(std::string text, float posY) {
    auto windowWidth = ImGui::GetWindowSize().x;
    auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::SetCursorPosY(posY);
    ImGui::Text(text.c_str());
}

void AlignForWidth(float width, float alignment = 0.5f)
{
    ImGuiStyle& style = ImGui::GetStyle();
    float avail = ImGui::GetContentRegionAvail().x;
    float off = (avail - width) * alignment;
    if (off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
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

            ImGui::SetNextWindowSize({ (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT });
            ImGui::SetNextWindowPos({ 0,0 });
            ImGui::SetNextWindowBgAlpha(0.f);
            ImGui::Begin("HUD", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

            ImGui::PushFont(font_SA_small);
            ImGui::SetCursorPosX(10);
            ImGui::SetCursorPosY(100);
            for (auto ship : player->shipArray)
            {
                if (ship->isActive)
                {
                    ImGui::Text(std::to_string(ship->health.healthStatus()).c_str());
                }
            }
			ImGui::PopFont();

			ImGui::PushFont(font_SA_large);
            
			ImVec2 planetHealthTextDim = ImGui::CalcTextSize(std::to_string(planet->health.healthStatus()).c_str());
            ImGui::SetCursorPos({ ImGui::GetWindowWidth() / 2 - planetHealthTextDim.x / 2, ImGui::GetWindowHeight() / 2 - planetHealthTextDim.y / 2 });
            ImGui::TextColored({ 0, 1, 0, 1 }, std::to_string(planet->health.healthStatus()).c_str());
            ImGui::PopFont();

            ImGui::PushFont(font_SA_medium);
            roundTimeText = TimerManager::GetTimer("Round Timer")->getHH_MM_SS_MS();
            auto windowWidth = ImGui::GetWindowSize().x;
            auto textWidth = ImGui::CalcTextSize("00:00:0,000").x;
            ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
            ImGui::SetCursorPosY(10);
            ImGui::Text(roundTimeText.c_str());
            ImGui::PopFont();

            ImGui::PushFont(font_SA_medium);
            ImGui::SetCursorPos( { 10, (float) SCREEN_HEIGHT - 70 } );
            scoreText = "Score: " + std::to_string(player->getScore());
            ImGui::Text(scoreText.c_str());
            
            ImGui::PopFont();


            ImGui::End();
            
            break;
        }
        case GameState::Pause:
        {
            Draw(lightingShader);

            ImGui::SetNextWindowSize({ (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT });
            ImGui::SetNextWindowPos({ 0,0 });
            ImGui::SetNextWindowBgAlpha(0.50f);
            ImGui::Begin("Pause Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

            ImGui::PushFont(font_SA_large);

            TextCentered("Pause", ImGui::GetWindowHeight() / 3);

            float buttonWidth = ImGui::CalcTextSize("Pause").x;
            ImGui::PopFont();
            ImGui::PushFont(font_SA_medium);
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) / 2);
            if (ImGui::Button("Resume", { buttonWidth, 0.f }))
            {
                ChangeGameState(GameState::Play);
            }

            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) / 2);
            if (ImGui::Button("Restart", { buttonWidth, 0.f }))
            {
                resetGame();
                ChangeGameState(GameState::Play);
            }

            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) / 2);
            if (ImGui::Button("Quit", { buttonWidth, 0.f }))
            {
                glfwSetWindowShouldClose(gameWindow, true);
            }

            ImGui::PopFont();
            ImGui::End();
            break;
        }
        case GameState::Menu:
        {
			Draw(lightingShader);
			drawMenuModel(lightingShader);

            ImGui::SetNextWindowSize({ (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT });
            ImGui::SetNextWindowPos({ 0,0 });            
            ImGui::SetNextWindowBgAlpha(0.15f);
            ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

            ImGui::PushFont(font_SA_menu);

            TextCentered("Space", ImGui::GetWindowHeight() / 4);
            TextCentered("Defender", ImGui::GetWindowHeight() / 4 + 60);

            float buttonWidth = ImGui::CalcTextSize("Defender").x;
            ImGui::PopFont();
            ImGui::PushFont(font_SA_large);
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) / 2);
            if (ImGui::Button("Play", { buttonWidth, 0.f }))
            {
                ChangeGameState(GameState::Play);
            }

            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) / 2);
            if (ImGui::Button("Quit", { buttonWidth, 0.f }))
            {
                glfwSetWindowShouldClose(gameWindow, true);
            }

            ImGui::PopFont();
            ImGui::End();
            break;
        }
        case GameState::GameOver:
        {
            //TextManager::Instance().RenderText("Menu", 300, 360, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f), "Space Age");

            // UI
            ImGui::SetNextWindowSize({ (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT });
            ImGui::SetNextWindowPos({ 0,0 });
            ImGui::Begin("Game Over", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

            ImGui::PushFont(font_SA_large);

            TextCentered("Game Over!", ImGui::GetWindowHeight() / 3);

            float buttonWidth = ImGui::CalcTextSize("game over").x;
            ImGui::PopFont();
            ImGui::PushFont(font_SA_medium);
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) / 2);
            if (ImGui::Button("Retry", { buttonWidth, 0.f }))
            {
                resetGame();
                ChangeGameState(GameState::Play);
            }

            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) / 2);
            if (ImGui::Button("Quit", { buttonWidth, 0.f }))
            {
                glfwSetWindowShouldClose(gameWindow, true);
            }

            ImGui::PopFont();
            ImGui::End();

            break;
        }
        case GameState::Shop:
        {
            Draw(lightingShader);

            ImGui::SetNextWindowSize({ (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT });
            ImGui::SetNextWindowPos({ 0,0 });
            ImGui::SetNextWindowBgAlpha(0.50f);
            ImGui::Begin("Shop", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

            ImGui::PushFont(font_SA_large);
            TextCentered("Shop", 100);
            ImGui::PopFont();

            ImGui::PushFont(font_SA_small);

            // ------------
            // Ship healing

			int healingCostFactor = 5;
            
			ImVec2 buttonDim = { ((ImGui::GetWindowWidth() - 20) / 4) - 5, 50};
            ImGui::SeparatorText("Ship Health");
            ImGui::SetCursorPosX(10);

            if (ImGui::BeginTable("table1", 4))
            {
                ImGui::TableNextRow();
                for (int i = 0; i < player->shipArray.size(); i++)
                {
                    ImGui::TableSetColumnIndex(i);
                    float progress = player->shipArray[i]->isActive ? (float)player->shipArray[i]->health.healthStatus() / (float)player->shipArray[i]->health.getMax() : 0.f;
                    ImGui::ProgressBar(progress, buttonDim);
                }
                ImGui::TableNextRow();

                for (int i = 0; i < player->shipArray.size(); i++)
                {
                    ImGui::TableSetColumnIndex(i);
					std::string buttonText = "Heal Ship " + std::to_string(i + 1);
					int healCost = (player->shipArray[i]->health.getMax() - player->shipArray[i]->health.healthStatus()) * healingCostFactor;
                    if (!player->shipArray[i]->isActive)
                        ImGui::BeginDisabled();
                    if (ImGui::Button(buttonText.c_str(), buttonDim) && player->getMoney() > healCost)
                    {
						player->addMoney(-healCost);
                        player->shipArray[i]->health.Heal(100);
                    }
                    if (!player->shipArray[i]->isActive)
                        ImGui::EndDisabled();
                }
                ImGui::EndTable();
            }

            ImGui::SeparatorText("Upgrades");
			ImVec2 upgradeButton = { 300, 100 };
			ImGui::SetCursorPosX(ImGui::GetWindowWidth()/4 - upgradeButton.x / 2);
			if (ImGui::Button("Add Ship\n500", upgradeButton) && player->getMoney() > 500)
			{
                for (int i = 0; i < player->shipArray.size(); i++)
				{
					if (!player->shipArray[i]->isActive)
					{
						player->addMoney(-500);
                        player->shipArray[i]->isActive = true;
						break;
					}
				}
			}
            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - upgradeButton.x / 2);
            if (ImGui::Button("Heal Planet\n200", upgradeButton) && player->getMoney() > 200)
            {
				player->addMoney(-200);
                planet->health.Heal(50);
            }
            ImGui::SameLine();
            ImGui::SetCursorPosX(3*ImGui::GetWindowWidth() / 4 - upgradeButton.x / 2);
            if (ImGui::Button("Placeholder\n1500", upgradeButton) && player->getMoney() > 0)
            {

            }

			TextCentered("Planet Health: " + std::to_string(planet->health.healthStatus()), ImGui::GetWindowHeight() / 2 + 100);

            ImGui::SetCursorPos({ 1.5f * buttonDim.x / 2, ImGui::GetWindowHeight() - 100 + buttonDim.y / 2 });
			string moneyText = "Money: " + std::to_string(player->getMoney());
            ImGui::Text(moneyText.c_str());

            ImGui::SetCursorPos({ ImGui::GetWindowWidth() - 3* buttonDim.x/2, ImGui::GetWindowHeight() - 100 });
            if (ImGui::Button("Continue", { ImGui::CalcTextSize(" Continue ").x , buttonDim.y }))
            {
                ChangeGameState(GameState::Play);
            }

            ImGui::PopFont();
            ImGui::End();
               
            break;
        }
    }
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

            CheckCoins(glm::vec3(worldCoordinates.x, worldCoordinates.y, 0.0));
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
    }
}

void Game::resetGame()
{
    SoundManager::Instance().stopAllSounds();
    roundTimer->resetTimer(true);
    player->resetPlayer();
    planet->resetPlanet();
    for (auto obj = activeObjects.begin(); obj != activeObjects.end(); )
    {
        if (dynamic_cast<Enemy*>(*obj) || dynamic_cast<Coin*>(*obj) || dynamic_cast<Projectile*>(*obj))
        {
            obj = activeObjects.erase(obj);
        }
        else
        {
            obj++;
        }
    }
    SoundManager::Instance().playSound("Assets/Sounds/star_striker.mp3", true);
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
    player->objectModel.Draw(lightingShader);
}