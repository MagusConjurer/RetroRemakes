#pragma once

#ifndef MESH
#include <GL\glew.h>

class Mesh {
public :
	Mesh();

	void createMesh(GLfloat* vertices,
					GLfloat* colors,
					unsigned int* indices,
					unsigned int numOfVertices,
					unsigned int numOfColors,
					unsigned int numOfIndices);
	void renderMesh();
	void clearMesh();

	~Mesh();

private:
	GLuint VAO; // vertex array object
	GLuint pos_VBO; // vertex buffer object
	GLuint clr_VBO;
	GLuint IBO; // index buffer object
	GLsizei indexCount;

};

#endif