#include <stdio.h>
#include <stdexcept>
#include <string.h>

#include "RetroRemakeWindow.h"

using std::runtime_error;
using rrdata::Color;

// Vertex Shader
static const char* vs_source = "        \n\
#version 330							\n\
                                        \n\
in vec3 pos;                            \n\
in vec4 clr;                            \n\
                                        \n\
out vec4 vColor;                        \n\
                                        \n\
void main() {                           \n\
    gl_Position = vec4(pos, 1.0);       \n\
    vColor = clr;                       \n\
}";

// Fragment Shader
static const char* fs_source = "        \n\
#version 330							\n\
                                        \n\
precision mediump float;                \n\
                                        \n\
in vec4 vColor;                         \n\
                                        \n\
out vec4 fColor;                        \n\
                                        \n\
void main() {                           \n\
    fColor = vColor;                    \n\
}";

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

    createTriangle();
    compileShaders();
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

    glUseProgram(shaderProgram);

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);

    glUseProgram(0);

    glfwSwapBuffers(mainWindow);
}

void RetroRemakeWindow::bindArrayData(GLfloat data[], GLuint VBO, GLuint numPerVertex) {
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // TODO: GL_STATIC_DRAW means we won't be changing the values, look into the dynamic option
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    // Start, num per, type, normalized, stride, offset
    glVertexAttribPointer(0, numPerVertex, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

void RetroRemakeWindow::createTriangle() {
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f
    };

    // Number of VAOs stored in VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    bindArrayData(vertices, pos_VBO, 3);
    bindArrayData(colors, clr_VBO, 4);

    // Unbind VBO(s) then VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void RetroRemakeWindow::addShader(GLuint program, const char* source, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);

    const GLchar* code[1];
    code[0] = source;

    GLint length[1];
    length[0] = strlen(source);

    glShaderSource(shader, 1, code, length);
    glCompileShader(shader);

    GLint result = 0;
    GLchar errorLog[1024] = { 0 };

    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(shader, sizeof(errorLog), NULL, errorLog);
        printf("%d shader | %s", shaderType, errorLog);
        throw runtime_error("Failed to compile shader! ( See log above )");
    }

    glAttachShader(program, shader);
}

void RetroRemakeWindow::compileShaders() {
    shaderProgram = glCreateProgram();

    if (!shaderProgram) {
        throw runtime_error("Failed to create shader program!");
    }

    addShader(shaderProgram, vs_source, GL_VERTEX_SHADER);
    addShader(shaderProgram, fs_source, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar errorLog[1024] = { 0 };

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
        printf("%s", errorLog);
        throw runtime_error("Failed to link shader program! ( See log above )");
    }

    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
        printf("%s", errorLog);
        throw runtime_error("Failed to validate shader program! ( See log above )");
    }
}

void RetroRemakeWindow::cleanup() {
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}
