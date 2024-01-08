#include "Mesh.h"

Mesh::Mesh() {
	VAO = 0;
	pos_VBO = 0;
    clr_VBO = 0;
	IBO = 0;
	indexCount = 0;
}

void Mesh::createMesh(  GLfloat* vertices,
                        GLfloat* colors,
                        unsigned int* indices,
                        unsigned int numOfVertices,
                        unsigned int numOfColors,
                        unsigned int numOfIndices) {
	indexCount = numOfIndices;

    // Number of VAOs stored in VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Set up for indexed draws
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0])*numOfIndices, indices, GL_STATIC_DRAW);

    // Bind vertices data
    glGenBuffers(1, &pos_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
    // TODO: GL_STATIC_DRAW means we won't be changing the values, may want to look into the dynamic option
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0])*numOfVertices, vertices, GL_STATIC_DRAW);

    // Start, num per, type, normalized, stride, offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Bind color data

    glGenBuffers(1, &clr_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, clr_VBO);
    // TODO: GL_STATIC_DRAW means we won't be changing the values, look into the dynamic option
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors[0])*numOfColors, colors, GL_STATIC_DRAW);

    // Start, num per, type, normalized, stride, offset
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // Unbind VBO(s), VAO, then IBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::renderMesh() {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::clearMesh() {
    if (IBO != 0) {
        glDeleteBuffers(1, &IBO);
        IBO = 0;
    }

    if (VAO != 0) {
        glDeleteBuffers(1, &VAO);
        VAO = 0;
    }

    if (pos_VBO != 0) {
        glDeleteBuffers(1, &pos_VBO);
        pos_VBO = 0;
    }

    if (clr_VBO != 0) {
        glDeleteBuffers(1, &clr_VBO);
        clr_VBO = 0;
    }

    indexCount = 0;
}

Mesh::~Mesh() {
	clearMesh();
}
