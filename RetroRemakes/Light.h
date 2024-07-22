#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

#ifndef LIGHT
#define LIGHT

class Light {
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat intensity);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="ambientIntensityLocation">Shader location for the intensity</param>
	/// <param name="ambientColorLocation">Shader location for the color</param>
	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation);

	~Light();

private:
	glm::vec3 color;
	GLfloat ambientIntensity;
};

#endif

