#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#include "Texture.h"

Texture::Texture(const std::string& image)
{
	glGenTextures(1, &TEX);
	glBindTexture(GL_TEXTURE_2D, TEX);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char *image_data = stbi_load(image.c_str(), &width, &height, &nrChannels, 0);

	if (image_data != NULL)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Error while loading image" << std::endl;
	}

	stbi_image_free(image_data);
}

Texture::~Texture()
{
}

void Texture::Use()
{
	glBindTexture(GL_TEXTURE_2D, TEX);
}


unsigned int Texture::getID(){
	return TEX;
}