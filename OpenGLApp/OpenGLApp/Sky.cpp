#include "Sky.h"
#include "MiniEngine/assimp_API/stb_image.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

unsigned int *thisVBO, *thisVAO;

unsigned int texture;
float vertices[] = {
	// positions          // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
	 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
	-0.5f,  0.5f, 0.0f,   0.0f, 1.0f, // top left 

	 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
	-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
};
unsigned int indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};
unsigned int VAO, VBO;

Sky::Sky()
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// load and create a texture 
	// -------------------------
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); 

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	unsigned char* data = stbi_load("Assets/Models/sky.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void Sky::Draw()
{
	// bind Texture
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}