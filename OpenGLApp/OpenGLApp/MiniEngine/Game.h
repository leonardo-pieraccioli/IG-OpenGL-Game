#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include<list>
#include"GameObject.h"
#include "../shader_s.h"

// Singleton
class Game final
{
public:
	~Game() = default;							// destructor
	Game(const Game& obj) = delete;				// removal of copy constructor 
	void operator=(const Game&) = delete;		// removal of copy assignment
	Game(Game&&) noexcept = default;			// move constructor
	Game& operator=(Game&&) noexcept = default; // move assignment
	
	// -------------------
	// game loop and setup 
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	const char* GAME_NAME;
	static Game &Instance();
	GLFWwindow* Setup(int screenWidth, int screenHeight, std::string gameName);
	void Update(float deltaTime);
	void Draw(Shader ourShader);

	// ---------------------
	// GameObject management
	bool InstantiateGameObject(GameObject* newGameObject, Transform* spawnTransform);
	void DestroyGameObject(GameObject& gameObject);

private:
	Game() 
	{

	}; 	
	std::list<GameObject*> activeObjects;
};

