#pragma once

#ifndef SHADER
#define SHADER

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

#include <GL/glew.h>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "CommonValues.h"

class Shader {
public:
	Shader();

	void CreateFromStrings(const char* vs_source, const char* fs_source);
	void CreateFromFiles(const char* vs_dir, const char* fs_dir);

	std::string ReadFile(const char* fileDir);

	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetProjectionLocation();
	GLuint GetEyePositionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetShininessLocation();

	void SetDirectionalLight(DirectionalLight *dLight);
	void SetPointLights(PointLight* pLights, unsigned int lightCount);

	void UseShader();
	void ClearShader();

	~Shader();
private:
	GLuint shaderID;
	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformProjection;
	GLuint uniformEyePosition;
	GLuint uniformSpecularIntensity;
	GLuint uniformShininess;
	GLuint uniformPointLightCount;

	int pointLightCount;

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLights[MAX_POINT_LIGHTS];

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint program, const char* source, GLenum shaderType);
};

#endif