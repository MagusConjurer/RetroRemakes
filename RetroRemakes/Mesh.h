#pragma once

#ifndef MESH
#define MESH

#include <GL\glew.h>

class Mesh {
public:
	Mesh();

	void CreateMesh(GLfloat* vertices,
					GLfloat* colors,
					unsigned int* indices,
					unsigned int numOfVertices,
					unsigned int numOfColors,
					unsigned int numOfIndices);
	void RenderMesh();
	void ClearMesh();

	~Mesh();

private:
	GLuint VAO; // vertex array object
	GLuint pos_VBO; // vertex buffer object
	GLuint clr_VBO;
	GLuint IBO; // index buffer object
	GLsizei indexCount;

};

#endif