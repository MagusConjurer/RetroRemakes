#include "Object.h"

#include <stdexcept>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::runtime_error;

Object::Object() {
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

    Mesh* pyramid = new Mesh();
    pyramid->createMesh(vertices, colors, indices, 15, 20, 18);
}

void Object::SetMesh(Mesh* mesh) {
    if(!mesh) {
        throw runtime_error("Failed to set mesh!");
    }

    objMesh = mesh;
}

void Object::Update() {
    if (objMesh) {
        objMesh->renderMesh();
    }
}

Object::~Object() {}
