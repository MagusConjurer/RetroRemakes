#include "RRWindow.h"

#include <stdexcept>
#include <stdio.h>
#include <string.h>

using std::runtime_error;

RRWindow::RRWindow() {
	InitialSetup(1080, 720);
}

RRWindow::RRWindow(GLint windowWidth, GLint windowHeight) {
	InitialSetup(windowWidth, windowHeight);
}

void RRWindow::InitialSetup(int windowWidth, int windowHeight) {
	width = windowWidth;
	height = windowHeight;

	mouseXLast = 0.0f;
	mouseYLast = 0.0f;
	mouseXChange = 0.0f;
	mouseYChange = 0.0f;

	for (size_t i = 0; i < 1024; i++) {
		keys[i] = false;
	}
}

void RRWindow::SetViewport() {
	glViewport(0, 0, bufferWidth, bufferHeight);
}

GLfloat RRWindow::GetBufferWidth() {
	return bufferWidth;
}

GLfloat RRWindow::GetBufferHeight() {
	return bufferHeight;
}

bool RRWindow::GetShouldClose() {
	return glfwWindowShouldClose(mainWindow);
}

bool* RRWindow::GetStateOfKeys() {
	return keys;
}

GLfloat RRWindow::GetMouseXChange() {
	GLfloat currentChange = mouseXChange;
	mouseXChange = 0.0f;
	return currentChange;
}

GLfloat RRWindow::GetMouseYChange() {
	GLfloat currentChange = mouseYChange;
	mouseYChange = 0.0f;
	return currentChange;
}

void RRWindow::SwapBuffers() {
	glfwSwapBuffers(mainWindow);
}

void RRWindow::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode) {
	RRWindow* thisWindow = static_cast<RRWindow*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// Is a valid key?
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			thisWindow->keys[key] = true;
		} else if (action == GLFW_RELEASE) {
			thisWindow->keys[key] = false;
		}
	}
}

void RRWindow::CreateCallbacks() {
	glfwSetKeyCallback(mainWindow, HandleKeys);
	glfwSetCursorPosCallback(mainWindow, HandleMouse);
}

void RRWindow::HandleMouse(GLFWwindow* window, double xPosition, double yPosition) {
	RRWindow* thisWindow = static_cast<RRWindow*>(glfwGetWindowUserPointer(window));

	if (thisWindow->mouseFirstMove) {
		thisWindow->mouseXLast = xPosition;
		thisWindow->mouseYLast = yPosition;
		thisWindow->mouseFirstMove = false;
	}

	thisWindow->mouseXChange = xPosition - thisWindow->mouseXLast;
	
	if (thisWindow->shouldInvertMouse) {
		thisWindow->mouseYChange = yPosition - thisWindow->mouseYLast;
	} else {
		thisWindow->mouseYChange = thisWindow->mouseYLast - yPosition;
	}

	thisWindow->mouseXLast = xPosition;
	thisWindow->mouseYLast = yPosition;
}

void RRWindow::InitWindow() {
	// Initialize GLFW and handle failure
	if (!glfwInit()) {
		glfwTerminate();
		throw runtime_error("GLFW initialization failed!");
	}

	// Window Properties
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "Retro Remakes", NULL, NULL);
	if (!mainWindow) {
		glfwTerminate();
		throw runtime_error("Failed to create GLFW window!");
	}

	// Buffer size info
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	CreateCallbacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Context for GLEW
	glfwMakeContextCurrent(mainWindow);
	// Allow modern extensions
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		Cleanup();
		throw runtime_error("Failed to initialize GLEW!");
	}

	// Enable depth buffer so correct triangles are drawn on top
	glEnable(GL_DEPTH_TEST);

	SetViewport();

	glfwSetWindowUserPointer(mainWindow, this);
}

void RRWindow::Cleanup() {
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

RRWindow::~RRWindow() {
	Cleanup();
}

