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

	// Shader details
	GLuint VAO; // vertex array object
	GLuint pos_VBO; // vertex buffer object
	GLuint clr_VBO;
	GLuint shaderProgram;

	void initWindow();

	void mainLoop();
	void drawFrame();

	void bindArrayData(GLfloat data[], GLuint VBO, GLuint numPerVertex);
	void createTriangle();
	void addShader(GLuint program, const char* source, GLenum shaderType);
	void compileShaders();

	void cleanup();
};

