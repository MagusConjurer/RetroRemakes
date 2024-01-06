#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class RRWindow {
public:
	RRWindow();
	void run();
private:
	// GLFW window details
	const GLint WIDTH = 1080;
	const GLint HEIGHT = 720;
	int bufferWidth;
	int bufferHeight;
	GLFWwindow* mainWindow;
	//rrdata::Color background{ 0.0f, 0.0f, 0.0f, 1.0f };

	// Shader details
	GLuint VAO; // vertex array object
	GLuint pos_VBO; // vertex buffer object
	GLuint clr_VBO;
	GLuint shaderProgram;
	GLint uniformModel;
	const float TORADIANS = 3.14159265f / 180.0f;

	// Angle in degrees
	float currentAngle = 0.0f;

	// Functions
	void initWindow();

	void bindArrayData(GLfloat data[], GLuint VBO, GLuint numPerVertex, GLuint location);
	void createTriangle();
	void addShader(GLuint program, const char* source, GLenum shaderType);
	void compileShaders();

	void updateModel();

	void mainLoop();
	void drawFrame();

	void cleanup();
};

