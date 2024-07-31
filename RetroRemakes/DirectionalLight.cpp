#include "DirectionalLight.h"

#include <glm/gtc/matrix_transform.hpp>

DirectionalLight::DirectionalLight() : Light() {
	// Default to straight down
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	lightProjection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
								   GLfloat xDir, GLfloat yDir, GLfloat zDir, GLuint shadowWidth, GLuint shadowHeight)
									: Light(red, green, blue, aIntensity, dIntensity, shadowWidth, shadowHeight) {
	direction = glm::vec3(xDir, yDir, zDir);
	lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
}

void DirectionalLight::UseLight(GLuint ambientIntensityLocation, GLuint colorLocation,
					 GLuint diffuseIntensityLocation, GLuint directionLocation) {
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform3f(colorLocation, color.x, color.y, color.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
}

glm::mat4 DirectionalLight::CalculateLightTransform() {
	// May need additional computation where straight up/down uses (0.0f, 0.0f, 1.0f) for the third parameter
	return lightProjection * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

DirectionalLight::~DirectionalLight() {

}