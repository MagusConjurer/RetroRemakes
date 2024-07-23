#pragma once

#ifndef SHADER
#define SHADER

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

#include <GL/glew.h>

class Shader {
public:
	Shader();

	void CreateFromStrings(const char* vs_source, const char* fs_source);
	void CreateFromFiles(const char* vs_dir, const char* fs_dir);

	std::string ReadFile(const char* fileDir);

	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetProjectionLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColorLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();
	GLuint GetEyePositionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetShininessLocation();

	void UseShader();
	void ClearShader();

	~Shader();
private:
	GLuint shaderID;
	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformProjection;
	GLuint uniformAmbientIntensity;
	GLuint uniformAmbientColor;
	GLuint uniformDiffuseIntensity;
	GLuint uniformDirection;
	GLuint uniformEyePosition;
	GLuint uniformSpecularIntensity;
	GLuint uniformShininess;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint program, const char* source, GLenum shaderType);
};

#endif