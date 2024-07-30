#pragma once

#include <GL\glew.h>

#include "stb_image.h"

#ifndef TEXTURE
#define TEXTURE
class Texture {
public:
	Texture();
	Texture(const char* fileLoc);

	bool LoadTextureRGB();
	bool LoadTextureRGBA();
	void UseTexture();
	void ClearTexture();

	~Texture();

private:
	GLuint textureID;
	int width;
	int height;
	int bitDepth;

	const char* fileLocation;
};

#endif
