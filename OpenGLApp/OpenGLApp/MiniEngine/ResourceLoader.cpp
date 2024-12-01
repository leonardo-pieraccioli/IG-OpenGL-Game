#include "ResourceLoader.h"
#include "../stb_image.h"
#include <glad/glad.h>

unsigned int LoadTexture(std::string filepath, bool hasAlpha) {
    // load and create a texture 
    // -------------------------
    unsigned int texture;
    // texture 1
    // ---------
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    std::string completePath = "Assets/Textures/" + filepath;
    unsigned char* data = stbi_load(completePath.c_str(), &width, &height, &nrChannels, 0);
    unsigned int imageFormat = hasAlpha ? GL_RGBA : GL_RGB;

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture: " << completePath << std::endl;
    }
    stbi_image_free(data);

    return texture;
}
