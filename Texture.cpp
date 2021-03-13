#include "Texture.h"



Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = (char*)"";
}

Texture::Texture(char* fileLocation)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	this->fileLocation = fileLocation;
}

Texture::Texture(char* fileLocation, GLuint ID)
{
	textureID = ID;
	width = 0;
	height = 0;
	bitDepth = 0;
	this->fileLocation = fileLocation;
}

GLuint Texture::getTextureID() {
	return textureID;
}

void Texture::loadTexture() {

	// Load in image
	unsigned char* textureData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);

	// Error checking
	if (!textureData) {
		printf("Failed to load texture.");
		return;
	}

	// Generate and bind texture

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Set parameters of the texture

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S, // Wrap along x-axis
		GL_REPEAT // Wrap by repeating the texture
	);

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T, // Wrap along y-axis
		GL_REPEAT // Wrap by repeating the texture
	);

	// Set filter to linear (as opposed to 'near')
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER,
		GL_NEAREST
	);

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER,
		GL_LINEAR
	);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind and free

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(textureData);

}

void Texture::useTexture() {

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

}


void Texture::clearTexture() {

	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = 0;

}


Texture::~Texture()
{
}
