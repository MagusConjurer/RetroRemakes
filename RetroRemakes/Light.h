#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

#ifndef LIGHT
#define LIGHT

class Light {
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
		  GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity);

	void UseLight(GLuint ambientIntensityLocation, GLuint colorLocation,
				  GLuint diffuseIntensityLocation, GLuint directionLocation);

	~Light();

private:
	glm::vec3 color;
	GLfloat ambientIntensity;

	glm::vec3 direction;
	GLfloat diffuseIntensity;
};

#endif

