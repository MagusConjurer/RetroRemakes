#include "Shader.h"

#include <stdexcept>
using std::runtime_error;
using std::string;

Shader::Shader() {
	shaderID = 0;
	uniformModel = 0;
	uniformView = 0;
	uniformProjection = 0;
	uniformAmbientColor = 0;
	uniformAmbientIntensity = 0;
	uniformDiffuseIntensity = 0;
	uniformDirection = 0;
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

GLuint Shader::GetAmbientIntensityLocation() {
	return uniformAmbientIntensity;
}

GLuint Shader::GetAmbientColorLocation() {
	return uniformAmbientColor;
}

GLuint Shader::GetDiffuseIntensityLocation() {
	return uniformDiffuseIntensity;
}

GLuint Shader::GetDirectionLocation() {
	return uniformDirection;
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
	uniformAmbientColor = 0;
	uniformAmbientIntensity = 0;
	uniformDiffuseIntensity = 0;
	uniformDirection = 0;
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
	uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.ambientIntensity");
	uniformAmbientColor = glGetUniformLocation(shaderID, "directionalLight.color");
	uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.diffuseIntensity");
	uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
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
