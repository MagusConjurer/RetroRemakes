#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>

#include <iostream>
#include <stdexcept>

#include "CommonValues.h"
#include "RRDataStructures.h"
#include "RRWindow.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"

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

// Directional Shadow Map Shaders
static const char* dsvs_source = "Shaders/directional_shadow_map_vert.shader";
static const char* dsfs_source = "Shaders/directional_shadow_map_frag.shader";

RRWindow window;
std::vector<Object*> objects;
std::vector<Shader*> shaders;
Shader directionalShadowShader;
Camera camera;

Texture leavesTexture;
Texture defaultTexture;
Material shinyMaterial;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
unsigned int pointLightCount = 0;
SpotLight spotLights[MAX_SPOT_LIGHTS];
unsigned int spotLightCount = 0;

// Very basic implementation
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Angle in degrees
float currentAngle = 0.0f;

Color defaultBackground = { 0.0f, 0.0f, 0.0f, 1.0f };

void CreateTextures() {
	leavesTexture = Texture("Textures/green-plant-leaves-512x512.png");
	leavesTexture.LoadTextureRGBA();

	defaultTexture = Texture("Textures/default.png");
	defaultTexture.LoadTextureRGBA();
}

void CreateMaterials() {
	shinyMaterial = Material(1.0f, 32.0f);
}

void CreateObjects() {

	CreateMaterials();

	Object* floor = new Object();
	floor->SetModel("Models/cube.obj");
	floor->SetMaterial(&shinyMaterial);
	floor->SetTranslation(0.0f, -1.0f, 0.0f);
	floor->SetScaling(10.0f, 1.0f, 10.0f);
	objects.push_back(floor);

	Object* cat = new Object();
	cat->SetModel("Models/12221_Cat_v1_l3.obj");
	cat->SetTranslation(0.0f, 0.0f, -5.0f);
	cat->SetRotation(-90.0f, 0.0f, 0.0f);
	cat->SetScaling(0.1f, 0.1f, 0.1f);
	objects.push_back(cat);
}

void CreateShaders() {
	Shader* mainShader = new Shader();
	mainShader->CreateFromFiles(vs_source, fs_source);
	shaders.push_back(mainShader);

	directionalShadowShader = Shader();
	directionalShadowShader.CreateFromFiles(dsvs_source, dsfs_source);
}

void RenderScene(Shader* shader) {
	for (Object* obj : objects) {
		glUniformMatrix4fv(shader->GetModelLocation(), 1, GL_FALSE, value_ptr(obj->GetTransformMatrix()));
		obj->GetMaterial()->UseMaterial(shader->GetSpecularIntensityLocation(), shader->GetShininessLocation());
		obj->Update();
	}
}

void DirectionalShadowMapPass(DirectionalLight* light) {
	directionalShadowShader.UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	mat4 transform = light->CalculateLightTransform();
	directionalShadowShader.SetDirectionalLightTransform(&transform);

	RenderScene(&directionalShadowShader);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 projection, glm::mat4 view) {
	shaders[0]->UseShader();

	// Reset viewport to the window settings
	window.SetViewport();

	glClearColor(defaultBackground.r, defaultBackground.g, defaultBackground.b, defaultBackground.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vec3 eyePos = camera.getCameraPosition();

	glUniformMatrix4fv(shaders[0]->GetViewLocation(), 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(shaders[0]->GetProjectionLocation(), 1, GL_FALSE, value_ptr(projection));
	glUniform3f(shaders[0]->GetEyePositionLocation(), eyePos.x, eyePos.y, eyePos.z);

	shaders[0]->SetDirectionalLight(&mainLight);
	shaders[0]->SetPointLights(pointLights, pointLightCount);
	shaders[0]->SetSpotLights(spotLights, spotLightCount);
	mat4 transform = mainLight.CalculateLightTransform();
	shaders[0]->SetDirectionalLightTransform(&transform);

	mainLight.GetShadowMap()->Read(GL_TEXTURE1);
	shaders[0]->SetTexture(0);
	shaders[0]->SetDirectionalShadowMap(1);

	vec3 flashlightPos = camera.getCameraPosition();
	flashlightPos.y -= 0.3f;
	spotLights[0].Follow(flashlightPos, camera.getCameraDirection());

	RenderScene(shaders[0]);
}

int main() {
	try {
		window = RRWindow();

		window.InitWindow();

		CreateObjects();
		CreateShaders();

		camera = Camera(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);
		
		mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 0.1f, 0.8f,
									 0.0f, -5.0f, 0.0f, 2048, 2048);
		
		pointLightCount = 0;

		pointLights[0] = PointLight(1.0, 0.0, 0.0f, 1.0f, 1.0f,
									-4.0f, 0.0f, 0.0f, 3.0f, 0.2f, 0.1f);
		pointLightCount++;

		pointLights[1] = PointLight(0.0, 1.0, 0.0f, 1.0f, 1.0f,
									4.0f, 0.0f, 0.0f, 3.0f, 0.2f, 0.1f);
		pointLightCount++;

		spotLightCount = 0;
		spotLights[0] = SpotLight(1.0, 1.0, 1.0f, 1.0f, 1.0f,
								  0.0f, 1.0f, -2.0f, 0.0f, -1.0f, 0.0f,
								  3.0f, 0.0f, 0.0f, 20.0f);
		//spotLightCount++;

		mat4 projection{ 1.0f };
		projection = perspective(
			45.0f, window.GetBufferWidth() / window.GetBufferHeight(), 0.1f, 100.0f);

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

			DirectionalShadowMapPass(&mainLight);

			RenderPass(projection, camera.calculateViewMatrix());

			glUseProgram(0);

			window.SwapBuffers();
		}
	}
	catch (const exception& except) {
		cerr << except.what() << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}