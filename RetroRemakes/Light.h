#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

#include "ShadowMap.h"

#ifndef LIGHT
#define LIGHT

class Light {
public:
	Light();

	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
		  GLuint shadowWidth, GLuint shadowHeight);

	ShadowMap* GetShadowMap() { return shadowMap; }

	~Light();

protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

	ShadowMap* shadowMap;
	glm::mat4 lightProjection;
};

#endif

