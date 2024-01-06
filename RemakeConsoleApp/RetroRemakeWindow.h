#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "RRDataStructures.h"

class RetroRemakeWindow {
public:
	RetroRemakeWindow();
	void run();
private:
	// GLFW window details
	const GLint WIDTH = 1080;
	const GLint HEIGHT = 720;
	int bufferWidth;
	int bufferHeight;
	GLFWwindow* mainWindow;
	rrdata::Color background{ 0.0f, 0.0f, 0.0f, 1.0f };

	void initWindow();

	void mainLoop();
	void drawFrame();

	void cleanup();
};

