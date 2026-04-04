#pragma once

#include <map>
#include <string>
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "ResourceManager.h"

#include "Shader.h"

class TextManager
{
private:
    TextManager() {}
public:
    struct Character {
            unsigned int TextureID; // ID handle of the glyph texture
            glm::ivec2   Size;      // Size of glyph
            glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
            unsigned int Advance;   // Horizontal offset to advance to next glyph
    };

    static TextManager& Instance();

    // Init
    void InitManager(int ScreenWidth, int ScreenHeight);

    int LoadFont(const char* fontName, std::string name);

    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color, std::string fontName);

    // properly de-allocates all loaded resources
    //static void Clear(); //TO IMPLEMENT
    
    std::map<std::string, std::map<GLchar, Character>> Fonts;

    Shader textShader;
    glm::mat4 textProjection;

    // VAOs and VBOs
    unsigned int VAOtext, VBOtext;

    int VAOandVBOInit;
};

