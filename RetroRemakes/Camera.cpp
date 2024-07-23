#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat defaultMovementSpeed, GLfloat defaultRotationSpeed) {
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	movementSpeed = defaultMovementSpeed;
	rotationSpeed = defaultRotationSpeed;

	update();
}

void Camera::keyControl(GLfloat deltaTime, bool* keys) {
	GLfloat velocity = movementSpeed * deltaTime;

	// TODO: Allow input to be set separately
	if (keys[GLFW_KEY_W]) {
		position += front * velocity;
	}

	if (keys[GLFW_KEY_S]) {
		position -= front * velocity;
	}

	if (keys[GLFW_KEY_D]) {
		position += right * velocity;
	}

	if (keys[GLFW_KEY_A]) {
		position -= right * velocity;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange) {
	xChange *= rotationSpeed;
	yChange *= rotationSpeed;

	yaw += xChange;
	pitch += yChange;

	// Limit the pitch
	if (pitch > 89.0f) {
		pitch = 89.0f;
	}

	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	update();
}

glm::vec3 Camera::getCameraPosition() {
	return position;
}

glm::mat4 Camera::calculateViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

void Camera::update() {
	// Based on yaw and pitch, calculate the various directions
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera() {

}