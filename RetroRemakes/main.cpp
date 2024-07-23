#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>

#include <iostream>
#include <stdexcept>

#include "RRDataStructures.h"
#include "RRWindow.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"

using rrdata::Color;

using glm::mat4;
using glm::perspective;
using glm::rotate;
using glm::scale;
using glm::translate;
using glm::value_ptr;
using glm::vec3;
using glm::cross;
using glm::normalize;
using std::cerr;
using std::endl;
using std::exception;

// Vertex Shader
static const char* vs_source = "Shaders/vertex.shader";

// Fragment Shader
static const char* fs_source = "Shaders/fragment.shader";

RRWindow window;
std::vector<Object*> objects;
std::vector<Shader*> shaders;
Camera camera;

Texture leavesTexture;

Light mainLight;

// Very basic implementation
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

const float TORADIANS = 3.14159265f / 180.0f;

// Angle in degrees
float currentAngle = 0.0f;

Color defaultBackground = { 0.0f, 0.0f, 0.0f, 1.0f };

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, 
						GLfloat* vertices, unsigned int verticeCount,
						unsigned int vLength, unsigned int normalOffset) {
	for (size_t i = 0; i < indiceCount; i += 3) {
		// Calculate index of each value from indices in the vertices
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;

		//      x							   y									  z
		vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		// Use cross product to get perpendicular vector to the two
		vec3 normal = cross(v1, v2);
		normal = normalize(normal);

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;

		vertices[in0] += normal.x;
		vertices[in0 + 1] += normal.y;
		vertices[in0 + 2] += normal.z;

		vertices[in1] += normal.x;
		vertices[in1 + 1] += normal.y;
		vertices[in1 + 2] += normal.z;

		vertices[in2] += normal.x;
		vertices[in2 + 1] += normal.y;
		vertices[in2 + 2] += normal.z;
	}

	for (size_t j = 0; j < verticeCount / vLength; j++) {
		unsigned int offset = j * vLength + normalOffset;
		vec3 vec(vertices[offset], vertices[offset + 1], vertices[offset + 2]);
		vec = normalize(vec);
		vertices[offset] = vec.x;
		vertices[offset + 1] = vec.y;
		vertices[offset + 2] = vec.z;
	}
}

void CreateObjects() {
	// Create pyramid
	uint32_t indices[] = {
		0, 1, 2,
		0, 1, 4,
		1, 2, 4,
		2, 3, 0,
		2, 3, 4,
		3, 0, 4
	};

	GLfloat vertices[] = {
	//  x      y      z     u     v			normals placeholder
		-1.0f, 0.0f,  1.0f, 0.25f, 0.25f,	0.0f, 0.0f, 0.0f,
		1.0f,  0.0f, 1.0f,	0.75f,  0.25f,	0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, -1.0f,	0.75f, 0.75f,	0.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, -1.0f, 0.25f, 0.75f,	0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,	0.5f, 0.5f,		0.0f, 0.0f, 0.0f
	};

	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.5f, 0.5f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f
	};

	calcAverageNormals(indices, 18, vertices, 40, 8, 5);

	Object* pyramid = new Object();
	Mesh* pyramidMesh = new Mesh();
	pyramidMesh->CreateMesh(vertices, colors, indices, 40, 20, 18);
	pyramid->SetMesh(pyramidMesh);
	objects.push_back(pyramid);
}

void CreateShaders() {
	Shader* mainShader = new Shader();
	mainShader->CreateFromFiles(vs_source, fs_source);
	shaders.push_back(mainShader);
}

void UpdateMVP() {
	mat4 model{ 1.0f };
	vec3 translation{ 0.0f, 0.0f, -2.5f };
	model = translate(model, translation);

	vec3 yAxis = vec3(0.0f, 1.0f, 0.0f);
	vec3 zAxis = vec3(0.0f, 0.0f, 1.0f);

	vec3 axis = yAxis;
	model = rotate(model, currentAngle * TORADIANS, axis);
	vec3 scaling{ 0.5f, 0.5f, 0.5f };
	model = scale(model, scaling);

	mat4 projection{ 1.0f };
	projection = perspective(
		45.0f, window.GetBufferWidth() / window.GetBufferHeight(), 0.1f, 100.0f);

	GLuint uniformModel = shaders[0]->GetModelLocation();
	GLuint uniformView = shaders[0]->GetViewLocation();
	GLuint uniformProjection = shaders[0]->GetProjectionLocation();
	GLuint uniformAmbientIntensity = shaders[0]->GetAmbientIntensityLocation();
	GLuint uniformAmbientColor = shaders[0]->GetAmbientColorLocation();
	GLuint uniformDiffuseIntensity = shaders[0]->GetDiffuseIntensityLocation();
	GLuint uniformDirection = shaders[0]->GetDirectionLocation();

	mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColor, uniformDiffuseIntensity, uniformDirection);

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, value_ptr(camera.calculateViewMatrix()));
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, value_ptr(projection));
}

void DrawFrame() {
	glClearColor(defaultBackground.r, defaultBackground.g, defaultBackground.b,
				 defaultBackground.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (Shader* shader : shaders) {
		shader->UseShader();
	}

	UpdateMVP();

	leavesTexture.UseTexture();

	for (Object* obj : objects) {
		obj->Update();
	}

	glUseProgram(0);

	window.SwapBuffers();
}

int main() {
	try {
		window = RRWindow();

		window.InitWindow();

		CreateObjects();
		CreateShaders();

		camera = Camera(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

		leavesTexture = Texture((char*)("Textures/green-plant-leaves-512x512.png"));
		leavesTexture.LoadTexture();

		mainLight = Light(1.0f, 1.0f, 1.0f, 0.2f, 
						  2.0f, -1.0f, 2.0f, 1.0f);

		// Loop until window closed
		while (!window.GetShouldClose()) {
			// Basic delta time
			GLfloat now = glfwGetTime();
			deltaTime = now - lastTime;
			lastTime = now;

			// Handle user input
			glfwPollEvents();

			camera.keyControl(deltaTime, window.GetStateOfKeys());
			camera.mouseControl(window.GetMouseXChange(), window.GetMouseYChange());

			currentAngle += 0.1f;
			if (currentAngle >= 360.0f) {
				currentAngle -= 360.0f;
			}

			DrawFrame();
		}
	}
	catch (const exception& except) {
		cerr << except.what() << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}