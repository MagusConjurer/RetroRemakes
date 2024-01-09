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
	bool* GetStateOfKeys();
	GLfloat GetMouseXChange();
	GLfloat GetMouseYChange(); 
	void SwapBuffers();
	~RRWindow();
private:
	// GLFW window details
	GLint width;
	GLint height;
	int bufferWidth;
	int bufferHeight;
	GLFWwindow* mainWindow;

	// Input
	bool keys[1024];
	GLfloat mouseXLast;
	GLfloat mouseYLast;
	GLfloat mouseXChange;
	GLfloat mouseYChange;
	bool mouseFirstMove;
	bool shouldInvertMouse = false;
	

	void InitialSetup(GLint windowWidth, GLint windowHeight);
	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void HandleMouse(GLFWwindow* window, double xPosition, double yPosition);
	/// <summary>
	/// Setup callbacks for key and mouse input from GLFW
	/// </summary>
	void CreateCallbacks();
	void Cleanup();
};

#endif