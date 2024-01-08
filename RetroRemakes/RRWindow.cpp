#include "RRWindow.h"

#include <stdio.h>
#include <stdexcept>
#include <string.h>

using std::runtime_error;

RRWindow::RRWindow() {
    width = 1080;
    height = 720;
}

RRWindow::RRWindow(GLint windowWidth, GLint windowHeight) {
    width = windowWidth;
    height = windowHeight;
}

GLfloat RRWindow::getBufferWidth() {
    return bufferWidth;
}

GLfloat RRWindow::getBufferHeight() {
    return bufferHeight;
}

bool RRWindow::getShouldClose() {
    return glfwWindowShouldClose(mainWindow);
}

void RRWindow::swapBuffers() {
    glfwSwapBuffers(mainWindow);
}

void RRWindow::initWindow() {
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

    // Context for GLEW
    glfwMakeContextCurrent(mainWindow);
    // Allow modern extensions
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        cleanup();
        throw runtime_error("Failed to initialize GLEW!");
    }

    // Enable depth buffer so correct triangles are drawn on top
    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, bufferWidth, bufferHeight);
}

void RRWindow::cleanup() {
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}

RRWindow::~RRWindow() {
    cleanup();
}