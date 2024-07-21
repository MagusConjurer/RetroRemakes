#pragma once

#include <GL\glew.h>

#include "stb_image.h"

#ifndef TEXTURE
#define TEXTURE
class Texture {
public:
	Texture();
	Texture(char* fileLoc);

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

	~Texture();

private:
	GLuint textureID;
	int width;
	int height;
	int bitDepth;

	char* fileLocation;
};

#endif
