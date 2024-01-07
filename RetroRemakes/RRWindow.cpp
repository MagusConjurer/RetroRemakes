#include "RRWindow.h"

#include <stdio.h>
#include <stdexcept>
#include <string.h>

using std::runtime_error;
//using rrdata::Color;
using glm::mat4;
using glm::vec3;
using glm::translate;
using glm::rotate;
using glm::scale;
using glm::value_ptr;

// Vertex Shader
static const char* vs_source = R"(            
#version 330							    

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 clr;

out vec4 vColor;

uniform mat4 model;

void main() {
    gl_Position = model * vec4(pos.x, pos.y, pos.z, 1.0);
    vColor = clr;
})";

// Fragment Shader
static const char* fs_source = R"(
#version 330

precision mediump float;

in vec4 vColor;

out vec4 fColor;

void main() {
    fColor = vColor;
})";


RRWindow::RRWindow() {}

void RRWindow::run() {
	initWindow();
	mainLoop();
	cleanup();
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

void RRWindow::createTriangle() {
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

    //bindArrayData(colors, clr_VBO, 4, 1);

    // Bind vertices data
    glGenBuffers(1, &pos_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
    // TODO: GL_STATIC_DRAW means we won't be changing the values, look into the dynamic option
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Start, num per, type, normalized, stride, offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Bind color data

    glGenBuffers(1, &clr_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, clr_VBO);
    // TODO: GL_STATIC_DRAW means we won't be changing the values, look into the dynamic option
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    // Start, num per, type, normalized, stride, offset
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // Unbind VBO(s) then VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void RRWindow::addShader(GLuint program, const char* source, GLenum shaderType) {
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

void RRWindow::compileShaders() {
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

    uniformModel = glGetUniformLocation(shaderProgram, "model");
}

void RRWindow::updateModel() {
    mat4 model{ 1.0f };
    vec3 translation = vec3(0.0f, 0.0f, 0.0f);
    model = translate(model, translation);
    vec3 zAxis = vec3(0.0f, 0.0f, 1.0f);
    model = rotate(model, currentAngle * TORADIANS, zAxis);
    // model = scale(model, scale);

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
}

void RRWindow::mainLoop() {
    // Loop until window closed
    while (!glfwWindowShouldClose(mainWindow)) {
        // Handle user input
        glfwPollEvents();

        currentAngle += 0.01f;
        if (currentAngle >= 360.0f) {
            currentAngle -= 360.0f;
        }

        drawFrame();
    }
}

void RRWindow::drawFrame() {
    glClearColor(background.r, background.g, background.b, background.a);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    updateModel();

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);

    glUseProgram(0);

    glfwSwapBuffers(mainWindow);
}

void RRWindow::cleanup() {
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}
