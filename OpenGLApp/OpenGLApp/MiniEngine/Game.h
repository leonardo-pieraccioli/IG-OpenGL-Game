#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <list>

#include "GameObject.h"
#include "../Shader.h"
#include "../Timer.h"
#include "SoundManager.h"
#include "../TextManager.h"
#include "../ResourceManager.h"
#include "../TimerManager.h"

#include "../Planet.h"
#include "../Player.h"
#include "../IObserver.h"


// Singleton
class Game final : IObserver
{
public:
	~Game()
	{
		while (!activeObjects.empty()) delete activeObjects.front(), activeObjects.pop_front();
	}							
	Game(const Game& obj) = delete;				// removal of copy constructor 
	void operator=(const Game&) = delete;		// removal of copy assignment
	Game(Game&&) noexcept = default;			// move constructor
	Game& operator=(Game&&) noexcept = default; // move assignment

	// ------------------------------
	// game information and reference
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	const char* GAME_NAME;
	static Game& Instance();

	// -------------------
	// game loop and setup 
	GLFWwindow* Setup(int screenWidth, int screenHeight, std::string gameName);
	void Init();
	void Update(float deltaTime);
	void ProcessInput(float deltaTime);
	void Draw(Shader shader);

	// ---------------------
	// GameObject management
	bool InstantiateGameObject(GameObject* newGameObject, Transform* spawnTransform); // , Model model);
	void DestroyGameObject(GameObject* gameObject);

	// ---------------------
	// Setters and Getters
	void setPlayer(Player* player);
	Player* getPlayer();
	// ----------
	// Game Logic
	void CheckCoins(glm::vec3 coinPosition);

	GameObject* CheckCollision(GameObject& caller, glm::vec3 position, glm::vec3 scale);

	// ----------------------------------
	// IObserver functions implementation
	void getNotified(std::string timerName, bool isCallbackEnabled) override;

	// -----------------
	// Permanent Objects
	Player* player;
	Planet* planet;

private:
	Game()
	{
		SCREEN_WIDTH = 0;
		SCREEN_HEIGHT = 0;
		GAME_NAME = "Game";
	};
	std::list<GameObject*> activeObjects;

	// VAOs and VBOs
	unsigned int VAOtext, VBOtext;
	unsigned int VBO, VAO;

	// matrices
	glm::mat4 textProjection;
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 lightPos = glm::vec3(10.0f, 2.0f, 12.0f);

	// camera and window parameters
	const unsigned int SCR_WIDTH = 1280;
	const unsigned int SCR_HEIGHT = 720;
	const float zNear = -20.0f;
	const float zFar = 20.0f;
	const float orthScale = 50.0f; // Parametro per zoomare e dezoomare gli oggetti con la camera ortografica

	// ENUM GameState
	enum class GameState { Play, Pause, Menu, GameOver, Shop };
	GameState gameState;

	// shaders
	Shader shader;
	Shader lightShader;
	Shader lightingShader;
	
	unsigned int text_coin;

	Timer* roundTimer;
	std::string roundTimeText;
	std::string scoreText;

	bool pHeldDown = false;
};

