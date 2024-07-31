#pragma once

#include "glm/glm.hpp"

#include "Model.h"

#ifndef OBJECT
#define OBJECT
class Object {
public:
	Object();

	void SetModel(const char* filename);
	void Update();

	void SetTranslation(GLfloat x, GLfloat y, GLfloat z);
	/// <summary>
	/// Set the rotation in degrees for each axis
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	void SetRotation(GLfloat x, GLfloat y, GLfloat z);

	/// <summary>
	/// Set the scaling in each axis
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	void SetScaling(GLfloat x, GLfloat y, GLfloat z);

	/// <summary>
	/// Used to get the transform (model) matrix for use in the MVP matrix
	/// </summary>
	/// <returns>A matrix with the object translation, rotation, and scale</returns>
	glm::mat4 GetTransformMatrix();

	~Object();
private:
	Model objModel;

	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scaling;

	
};

#endif
