#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include<list>
#include"GameObject.h"
#include "assimp_API/shader.h"

#include "../Planet.h"
#include "../Player.h"


// Singleton
class Game final
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
	static Game &Instance();

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

	// ----------
	// Game Logic
	void CheckCoins(glm::vec3 coinPosition);

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
};

