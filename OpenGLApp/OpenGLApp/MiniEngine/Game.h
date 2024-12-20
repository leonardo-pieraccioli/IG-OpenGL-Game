#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include<list>
#include"GameObject.h"
#include "../shader_s.h"

#include "../Player.h"
#include "SoundManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H

// Singleton
class Game final
{
public:
	~Game() = default;							// destructor
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
	void Update(float deltaTime);
	void Draw(Shader ourShader);
	int fontSetup();
	void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);

	// ---------------------
	// GameObject management
	bool InstantiateGameObject(GameObject* newGameObject, Transform* spawnTransform, unsigned int texture);
	bool InstantiateGameObject(GameObject* newGameObject, glm::vec3 position);
	void DestroyGameObject(GameObject* gameObject);

	void CheckCoins(glm::vec3 coinPosition, Player* ptr_player);

private:
	Game()
	{
		SCREEN_WIDTH = 0;
		SCREEN_HEIGHT = 0;
		GAME_NAME = "Game";
	};
	std::list<GameObject*> activeObjects;

	glm::mat4 textProjection;
};

