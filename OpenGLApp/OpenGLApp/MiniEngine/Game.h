#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include<list>
#include"GameObject.h"
#include "../shader_s.h"

#include "../Player.h"
#include "SoundManager.h"
#include "ResourceLoader.h"
#include "../ResourceManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H

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
	void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);

	// ---------------------
	// GameObject management
	bool InstantiateGameObject(GameObject* newGameObject, Transform* spawnTransform, unsigned int texture);
	bool InstantiateGameObject(GameObject* newGameObject, glm::vec3 position);
	void DestroyGameObject(GameObject* gameObject);

	// ---------------------
	// Setters and Getters
	void setPlayer(Player* player);
	Player* getPlayer();

	void CheckCoins(glm::vec3 coinPosition, Player* ptr_player);

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
	Shader ourShader;
	
	unsigned int text_coin;
	std::map<GLchar, Character> Characters;
	Player* player;
};

