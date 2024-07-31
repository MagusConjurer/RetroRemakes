#include "Shader.h"

#include <stdexcept>
#include <glm\gtc\type_ptr.hpp>
using std::runtime_error;
using std::string;

Shader::Shader() {
	shaderID = 0;
	uniformModel = 0;
	uniformView = 0;
	uniformProjection = 0;

	pointLightCount = 0;
}

void Shader::CreateFromStrings(const char* vertexCode, const char* fragmentCode) {
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vs_dir, const char* fs_dir) {
	string vertexCode = ReadFile(vs_dir);
	string fragmentCode = ReadFile(fs_dir);

	CreateFromStrings(vertexCode.c_str(), fragmentCode.c_str());
}

string Shader::ReadFile(const char* fileDir) {
	string content;
	std::ifstream fileStream(fileDir, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Failed to read file %s!", fileDir);
		throw runtime_error("File doesn't exist!");
	}

	string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();

	return content;
}

GLuint Shader::GetModelLocation() {
	return uniformModel;
}

GLuint Shader::GetViewLocation() {
	return uniformView;
}

GLuint Shader::GetProjectionLocation() {
	return uniformProjection;
}

GLuint Shader::GetEyePositionLocation() {
	return uniformEyePosition;
}

GLuint Shader::GetSpecularIntensityLocation() {
	return uniformSpecularIntensity;
}

GLuint Shader::GetShininessLocation() {
	return uniformShininess;
}

void Shader::SetDirectionalLight(DirectionalLight* dLight) {
	dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity,
					uniformDirectionalLight.uniformColor,
					uniformDirectionalLight.uniformDiffuseIntensity,
					uniformDirectionalLight.uniformDirection);
}

void Shader::SetPointLights(PointLight* pLights, unsigned int lightCount) {
	if (lightCount > MAX_POINT_LIGHTS) {
		lightCount = MAX_POINT_LIGHTS;
	}

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++) {
		pLights[i].UseLight(uniformPointLights[i].uniformAmbientIntensity,
							uniformPointLights[i].uniformColor,
							uniformPointLights[i].uniformDiffuseIntensity,
							uniformPointLights[i].uniformPosition,
							uniformPointLights[i].uniformConstant,
							uniformPointLights[i].uniformLinear,
							uniformPointLights[i].uniformExponent);
	}
}

void Shader::SetSpotLights(SpotLight* sLights, unsigned int spotLightCount) {
	if (spotLightCount > MAX_SPOT_LIGHTS) {
		spotLightCount = MAX_SPOT_LIGHTS;
	}

	glUniform1i(uniformSpotLightCount, spotLightCount);

	for (size_t i = 0; i < spotLightCount; i++) {
		sLights[i].UseLight(uniformSpotLights[i].uniformAmbientIntensity,
							uniformSpotLights[i].uniformColor,
							uniformSpotLights[i].uniformDiffuseIntensity,
							uniformSpotLights[i].uniformPosition,
							uniformSpotLights[i].uniformDirection,
							uniformSpotLights[i].uniformConstant,
							uniformSpotLights[i].uniformLinear,
							uniformSpotLights[i].uniformExponent,
							uniformSpotLights[i].uniformEdge);
	}
}

void Shader::SetTexture(GLuint textureUnit) {
	glUniform1i(uniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit) {
	glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4* transform) {
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*transform));
}

void Shader::UseShader() {
	glUseProgram(shaderID);
}

void Shader::ClearShader() {
	if (shaderID != 0) {
		glDeleteProgram(shaderID);
	}

	uniformModel = 0;
	uniformView = 0;
	uniformProjection = 0;
}

Shader::~Shader() {
	ClearShader();
}

void Shader::CompileShader(const char* vs_source, const char* fs_source) {
	shaderID = glCreateProgram();

	if (!shaderID) {
		throw runtime_error("Failed to create shader program!");
	}

	AddShader(shaderID, vs_source, GL_VERTEX_SHADER);
	AddShader(shaderID, fs_source, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar errorLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(errorLog), NULL, errorLog);
		printf("%s", errorLog);
		throw runtime_error("Failed to link shader program! ( See log above )");
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(errorLog), NULL, errorLog);
		printf("%s", errorLog);
		throw runtime_error("Failed to validate shader program! ( See log above )");
	}

	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");

	uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderID, "directionalLight.base.color");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");

	// Setup point lights
	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++) {
		char locBuff[100] = { '\0' };
		
		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
		uniformPointLights[i].uniformColor = glGetUniformLocation(shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
		uniformPointLights[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		uniformPointLights[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
		uniformPointLights[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
		uniformPointLights[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
		uniformPointLights[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
		uniformPointLights[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
	}

	// Setup spot lights
	uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

	for (size_t j = 0; j < MAX_SPOT_LIGHTS; j++) {
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.color", j);
		uniformSpotLights[j].uniformColor = glGetUniformLocation(shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", j);
		uniformSpotLights[j].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", j);
		uniformSpotLights[j].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", j);
		uniformSpotLights[j].uniformPosition = glGetUniformLocation(shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", j);
		uniformSpotLights[j].uniformConstant = glGetUniformLocation(shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", j);
		uniformSpotLights[j].uniformLinear = glGetUniformLocation(shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", j);
		uniformSpotLights[j].uniformExponent = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", j);
		uniformSpotLights[j].uniformDirection = glGetUniformLocation(shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", j);
		uniformSpotLights[j].uniformEdge = glGetUniformLocation(shaderID, locBuff);
	}

	uniformTexture = glGetUniformLocation(shaderID, "textureData");
	uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform");
	uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");
}

void Shader::AddShader(GLuint program, const char* source, GLenum shaderType) {
	GLuint shader = glCreateShader(shaderType);

	const GLchar* code[1];
	code[0] = source;

	GLint length[1];
	length[0] = strlen(source);

	glShaderSource(shader, 1, code, length);
	glCompileShader(shader);

	GLint result = 0;
	GLchar errorLog[1024] = { 0 };

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(shader, sizeof(errorLog), NULL, errorLog);
		printf("%d shader | %s", shaderType, errorLog);
		throw runtime_error("Failed to compile shader! ( See log above )");
	}

	glAttachShader(program, shader);
}
