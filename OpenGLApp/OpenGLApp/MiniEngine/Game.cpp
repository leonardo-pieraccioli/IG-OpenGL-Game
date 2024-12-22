#include "Game.h"

#include "../ResourceManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdexcept>

#include "../Coin.h"
#include "../Planet.h"

GLFWwindow* gameWindow;

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

    // Text Shader
    textShader = ResourceManager::LoadShader("textShader.vs", "textShader.fs", nullptr, "TextShader");
    //textShader = new Shader("textShader.vs", "textShader.fs");

  
    // CAMERA SETUP
    // ------------
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 15.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

    fontSetup();

    return gameWindow;
}


void Game::Init()
{
    player = new Player();
    planet = new Planet();
    InstantiateGameObject(planet, new Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.25f, 1.25f, 1.25f)));
    InstantiateGameObject(player, new Transform());
    // SpaceDefender.InstantiateGameObject(new Ship(), new Transform(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.f, 0.f, 0.f), shipScale));
}

void Game::Update(float deltaTime)
{
    
    // generazione monete
    Coin::generateCoins(deltaTime);

    for (auto obj = activeObjects.begin(); obj != activeObjects.end(); obj++)
    {
        (*obj)->Update(deltaTime);
    }
    Draw(shader);

    std::string scoreText = "Score: " + std::to_string(player->getMoney());
    RenderText(scoreText, 0, 0, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
}

void Game::Draw(Shader shader)
{
    shader.Use();

    // Mi trovo la matrice ortografica per la camera
    glm::mat4 projection = glm::ortho(-((float)SCR_WIDTH / 2), (float)SCR_WIDTH / 2, -((float)SCR_HEIGHT / 2), (float)SCR_HEIGHT / 2, zNear, zFar);

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

    double xpos, ypos;
    glfwGetCursorPos(gameWindow, &xpos, &ypos);

    if (glfwGetMouseButton(gameWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        // TEMPORARY: 12.5 and 6.5 are the approximate coordinates of the limits
        //            of what we can see with the camera.
        //            The function should map mouse coordinates more precisely
        // ---------------------------------------------------------------------
        // xpos : 1280 = xworld : 12.5 -> xworld = xpos * 12.5 / 1280
        double xworld = (xpos * 26 / SCREEN_WIDTH) - 13;
        double yworld = -((ypos * 13 / SCREEN_HEIGHT) - 6.5);
        // ---------------------------------------------------------------------

        // std::cout << "Mouse clicked in " << xworld << ":" << yworld << std::endl;
        CheckCoins(glm::vec3(xworld, yworld, 0.0));
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

	newGameObject->transform = *spawnTransform;
    //newGameObject->objectModel = model;
	activeObjects.push_back(newGameObject);
	return true;
}

void Game::DestroyGameObject(GameObject* gameObject)
{
	activeObjects.remove(gameObject);
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
    
    for (auto obj = activeObjects.begin(); obj != activeObjects.end(); )
    {
        Coin* coin = dynamic_cast<Coin*>(*obj);
        if (coin) {
            if (coin->shouldDestroy(coinPosition)) {  // coordinate da sostituire
                player->addMoney(coin->getMoney());
                SoundManager::Instance().playSound("Assets/Sounds/coin_pickup.mp3", false);
                obj = activeObjects.erase(obj);
                DestroyGameObject(coin);
            }
            else {
                ++obj;
            }
        }
        else {
            ++obj;
        }
    }
}

int Game::fontSetup()
{
    glm::mat4 textProjection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    textShader.Use();
    glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(textProjection));

    // FreeType
    // --------
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    // find path to font
    std::string font_name = "resources/fonts/Antonio/static/Antonio-Bold.ttf";
    //std::string font_name = "resources/fonts/Roboto/Roboto-Bold.ttf";
    if (font_name.empty())
    {
        std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
        return -1;
    }
    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);


    // configure VAO/VBO for texture quads
    // -----------------------------------
    glGenVertexArrays(1, &VAOtext);
    glGenBuffers(1, &VBOtext);
    glBindVertexArray(VAOtext);
    glBindBuffer(GL_ARRAY_BUFFER, VBOtext);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// render line of text
// -------------------
void Game::RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
{
    // activate corresponding render state
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    textShader.Use();
    glUniform3f(glGetUniformLocation(textShader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAOtext);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBOtext);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
