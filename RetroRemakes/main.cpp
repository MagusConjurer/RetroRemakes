#include <stdio.h>
#include <stdexcept>
#include <iostream>

#include "RRWindow.h"
#include "RRDataStructures.h"

using rrdata::Color;

using std::exception;
using std::cerr;
using std::endl;
using glm::mat4;
using glm::vec3;
using glm::translate;
using glm::rotate;
using glm::scale;
using glm::value_ptr;
using glm::perspective;

// Vertex Shader
static const char* vs_source = "Shaders/vertex.shader";

// Fragment Shader
static const char* fs_source = "Shaders/fragment.shader";

RRWindow window;
std::vector<Object*> objects;
std::vector<Shader*> shaders;

const float TORADIANS = 3.14159265f / 180.0f;

// Angle in degrees
float currentAngle = 0.0f;

Color defaultBackground = { 0.0f, 0.0f, 0.0f, 1.0f };

void createObjects() {
    // Create pyramid
    uint32_t indices[] = {
        0, 1, 2,
        0, 1, 4,
        1, 2, 4,
        2, 3, 0,
        2, 3, 4,
        3, 0, 4
    };

    GLfloat vertices[] = {
        -1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, -1.0f,
        -1.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f,
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f
    };

    Object* pyramid = new Object();
    Mesh* pyramidMesh = new Mesh();
    pyramidMesh->createMesh(vertices, colors, indices, 15, 20, 18);
    pyramid->SetMesh(pyramidMesh);
    objects.push_back(pyramid);
}

void createShaders() {
    Shader* mainShader = new Shader();
    mainShader->createFromFiles(vs_source, fs_source);
    shaders.push_back(mainShader);
}

void updateMVP() {
    mat4 model{ 1.0f };
    vec3 translation{ 0.0f, 0.0f, -2.5f };
    model = translate(model, translation);

    vec3 yAxis = vec3(0.0f, 1.0f, 0.0f);
    vec3 zAxis = vec3(0.0f, 0.0f, 1.0f);

    vec3 axis = yAxis;
    model = rotate(model, currentAngle * TORADIANS, axis);
    vec3 scaling{ 0.5f, 0.5f, 0.5f };
    model = scale(model, scaling);

    mat4 projection{ 1.0f };
    projection = perspective(45.0f, window.getBufferWidth()/ window.getBufferHeight(), 0.1f, 100.0f);

    GLuint uniformModel = shaders[0]->getModelLocation();
    GLuint uniformProjection = shaders[0]->getProjectionLocation();

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, value_ptr(projection));
}

void drawFrame() {
    glClearColor(defaultBackground.r, defaultBackground.g, defaultBackground.b, defaultBackground.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (Shader* shader : shaders) {
        shader->useShader();
    }

    updateMVP();

    for (Object* obj : objects) {
        obj->Update();
    }

    glUseProgram(0);

    window.swapBuffers();
}


int main() {
    try {
        window = RRWindow();

        window.initWindow();

        createObjects();
        createShaders();

        // Loop until window closed
        while (!window.getShouldClose()) {
            // Handle user input
            glfwPollEvents();

            currentAngle += 0.1f;
            if (currentAngle >= 360.0f) {
                currentAngle -= 360.0f;
            }

            drawFrame();
        }
    }
    catch (const exception& except) {
        cerr << except.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}