#include "Light.h"

Light::Light() {
	// Default to "white", how much of each color to show
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;

	// Default to straight down
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	diffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
			 GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity) {
	color = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;

	direction = glm::vec3(xDir, yDir, zDir);
	diffuseIntensity = dIntensity;
}

void Light::UseLight(GLuint ambientIntensityLocation, GLuint colorLocation,
					 GLuint diffuseIntensityLocation, GLuint directionLocation) {
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform3f(colorLocation, color.x, color.y, color.z);

	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
}

Light::~Light() {}