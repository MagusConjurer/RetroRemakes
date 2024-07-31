#pragma once

#include "Light.h"

#ifndef DIRECTIONAL_LIGHT
#define DIRECTIONAL_LIGHT
class DirectionalLight : public Light{
public:
	DirectionalLight();

	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
					 GLfloat xDir, GLfloat yDir, GLfloat zDir, GLuint shadowWidth, GLuint shadowHeight);

	void UseLight(GLuint ambientIntensityLocation, GLuint colorLocation,
				  GLuint diffuseIntensityLocation, GLuint directionLocation);

	glm::mat4 CalculateLightTransform();

	~DirectionalLight();
private:
	glm::vec3 direction;
};
#endif

