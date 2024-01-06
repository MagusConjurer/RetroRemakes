#include <stdio.h>
#include <stdexcept>

#include "RetroRemakeWindow.h"

using std::runtime_error;

RetroRemakeWindow::RetroRemakeWindow() {

}

void RetroRemakeWindow::run() {
    initWindow();
	mainLoop();
	cleanup();
}

void RetroRemakeWindow::initWindow() {
    // Initialize GLFW and handle failure
    if (!glfwInit()) {
        glfwTerminate();
        throw runtime_error("GLFW initialization failed!");
    }

    // Window Properties
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Retro Remakes", NULL, NULL);
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

    glViewport(0, 0, bufferWidth, bufferHeight);
}

void RetroRemakeWindow::mainLoop() {
    // Loop until window closed
    while (!glfwWindowShouldClose(mainWindow)) {
        // Handle user input
        glfwPollEvents();
        drawFrame();
    }
}

void RetroRemakeWindow::drawFrame() {
    glClearColor(background.r, background.g, background.b, background.a);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(mainWindow);
}

void RetroRemakeWindow::cleanup() {
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}
