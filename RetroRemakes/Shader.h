#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader {
public:
	Shader();

	void createFromStrings(const char* vs_source, const char* fs_source);
	void createFromFiles(const char* vs_dir, const char* fs_dir);

	std::string readFile(const char* fileDir);

	GLuint getModelLocation();
	GLuint getViewLocation();
	GLuint getProjectionLocation();
	void useShader();
	void clearShader();

	~Shader();
private:
	GLuint shaderID;
	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformProjection;

	void compileShader(const char* vertexCode, const char* fragmentCode);
	void addShader(GLuint program, const char* source, GLenum shaderType);


};

