#include "Object.h"

#include <stdexcept>
#include <glm/gtc/matrix_transform.hpp>
#include "CommonValues.h"

using std::runtime_error;
using glm::mat4;
using glm::vec3;
using glm::translate;
using glm::rotate;
using glm::scale;

Object::Object() {
	objModel = Model();
	objMaterial = new Material(0.3f, 4.0f);

	translation = { 0.0f, 0.0f, 0.0f };
	rotation = { 0.0f, 0.0f, 0.0f };
	scaling = { 1.0f, 1.0f, 1.0f };
}

Object::~Object() {
	objMaterial = nullptr;
}

void Object::SetModel(const char* filename) {
	if (!objModel.LoadModel(filename)) {
		throw runtime_error("Failed to set model!");
	}
}

void Object::SetMaterial(Material* material) {
	objMaterial = material;
}

Material* Object::GetMaterial() {
	return objMaterial;
}

void Object::Update() {
	objModel.RenderModel();
}

void Object::SetTranslation(GLfloat x, GLfloat y, GLfloat z) {
	translation = { x, y, z };
}

void Object::SetRotation(GLfloat x, GLfloat y, GLfloat z) {
	rotation = { x, y, z };
}

void Object::SetScaling(GLfloat x, GLfloat y, GLfloat z) {
	scaling = { x, y, z };
}

mat4 Object::GetTransformMatrix() {
	mat4 model { 1.0f };

	model = translate(model, translation);

	vec3 xAxis = vec3(1.0f, 0.0f, 0.0f);
	vec3 yAxis = vec3(0.0f, 1.0f, 0.0f);
	vec3 zAxis = vec3(0.0f, 0.0f, 1.0f);

	model = rotate(model, rotation.x * TORADIANS, xAxis);
	model = rotate(model, rotation.y * TORADIANS, yAxis);
	model = rotate(model, rotation.z * TORADIANS, zAxis);

	model = scale(model, scaling);

	return model;
}
