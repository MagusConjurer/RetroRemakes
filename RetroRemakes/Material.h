#pragma once

#include <GL\glew.h>

#ifndef MATERIAL
#define MATERIAL
class Material {
public:
	Material();
	/// <summary>
	/// 
	/// </summary>
	/// <param name="sIntensity"></param>
	/// <param name="shine">Usually a factor of 2, such as 32</param>
	Material(GLfloat sIntensity, GLfloat shine);


	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

	~Material();

private:
	GLfloat specularIntensity;
	GLfloat shininess;
};
#endif
