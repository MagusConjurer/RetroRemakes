#pragma once

#ifndef RRWINDOW
#define RRWINDOW

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "Object.h"
#include "RRDataStructures.h"
#include "Shader.h"

class RRWindow {
public:
	RRWindow();
	RRWindow(GLint windowWidth, GLint windowHeight);
	void InitWindow();
	GLfloat GetBufferWidth();
	GLfloat GetBufferHeight();
	bool GetShouldClose();
	void SwapBuffers();
	~RRWindow();
private:
	// GLFW window details
	GLint width;
	GLint height;
	int bufferWidth;
	int bufferHeight;
	GLFWwindow* mainWindow;

	void Cleanup();
};

#endif