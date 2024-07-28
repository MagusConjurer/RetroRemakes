#pragma once
#include "Light.h"

#ifndef POINT_LIGHT
#define POINT_LIGHT
class PointLight : public Light {
public:
	PointLight();
	PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
			   GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat con, GLfloat lin, GLfloat exp);

	void UseLight(GLuint ambientIntensityLocation, GLuint colorLocation,
				  GLuint diffuseIntensityLocation, GLuint positionLocation,
				  GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

	~PointLight();
protected:
	glm::vec3 position;

	GLfloat constant;
	GLfloat linear;
	GLfloat exponent;
};
#endif

