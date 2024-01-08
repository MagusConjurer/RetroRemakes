#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "RRDataStructures.h"
#include "Object.h"
#include "Shader.h"

class RRWindow {
public:
	RRWindow();
	RRWindow(GLint windowWidth, GLint windowHeight);
	void initWindow();
	GLfloat getBufferWidth();
	GLfloat getBufferHeight();
	bool getShouldClose();
	void swapBuffers();
	~RRWindow();
private:
	// GLFW window details
	GLint width;
	GLint height;
	int bufferWidth;
	int bufferHeight;
	GLFWwindow* mainWindow;

	void cleanup();
};

