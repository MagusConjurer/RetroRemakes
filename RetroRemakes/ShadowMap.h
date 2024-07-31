#pragma once

#include <stdio.h>
#include <GL\glew.h>

#ifndef SHADOW_MAP
#define SHADOW_MAP
class ShadowMap {
public:

	ShadowMap();

	virtual bool Init(GLuint width, GLuint height);

	virtual void Write();

	virtual void Read(GLenum textureUnit);

	GLuint GetShadowWidth() { return shadowWidth; };

	GLuint GetShadowHeight() { return shadowHeight; };

	~ShadowMap();

protected:
	GLuint FBO;
	GLuint shadowMap;
	GLuint shadowWidth;
	GLuint shadowHeight;
};

#endif