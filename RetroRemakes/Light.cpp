#include "Light.h"

Light::Light() {
	// Default to "white", how much of each color to show
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat intensity) {
	color = glm::vec3(red, green, blue);
	ambientIntensity = intensity;
}

void Light::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation) {
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
}

Light::~Light() {}