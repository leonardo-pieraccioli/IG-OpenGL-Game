#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <list>
#include <map>

#include "GameObject.h"
#include "../Shader.h"
#include "SoundManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../Planet.h"
#include "../Player.h"


// Singleton
class Game final
{
private:
	/// Holds all state information relevant to a character as loaded using FreeType
	struct Character {
		unsigned int TextureID; // ID handle of the glyph texture
		glm::ivec2   Size;      // Size of glyph
		glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
		unsigned int Advance;   // Horizontal offset to advance to next glyph
	};
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
	int fontSetup();
	void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);

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

	// camera and window parameters
	const unsigned int SCR_WIDTH = 1280;
	const unsigned int SCR_HEIGHT = 720;
	const float zNear = -20.0f;
	const float zFar = 20.0f;
	const float orthScale = 50.0f; // Parametro per zoomare e dezoomare gli oggetti con la camera ortografica

	// shaders
	Shader textShader;
	Shader shader;
	
	unsigned int text_coin;
	std::map<GLchar, Character> Characters;
};

