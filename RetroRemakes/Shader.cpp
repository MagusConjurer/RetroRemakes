#include "Shader.h"

#include <stdexcept>
using std::runtime_error;
using std::string;

Shader::Shader() {
	shaderID = 0;
	uniformModel = 0;
	uniformView = 0;
	uniformProjection = 0;
}

void Shader::createFromStrings(const char* vertexCode, const char* fragmentCode) {
	compileShader(vertexCode, fragmentCode);
}

void Shader::createFromFiles(const char* vs_dir, const char* fs_dir) {
    string vertexCode = readFile(vs_dir);
    string fragmentCode = readFile(fs_dir);

    createFromStrings(vertexCode.c_str(), fragmentCode.c_str());
}

string Shader::readFile(const char* fileDir) {
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

GLuint Shader::getModelLocation() {
	return uniformModel;
}

GLuint Shader::getViewLocation() {
	return uniformView;
}

GLuint Shader::getProjectionLocation() {
	return uniformProjection;
}

void Shader::useShader() {
    glUseProgram(shaderID);
}

void Shader::clearShader() {
    if (shaderID != 0) {
        glDeleteProgram(shaderID);
    }

    uniformModel = 0;
    uniformView = 0;
    uniformProjection = 0;
}

Shader::~Shader() {
    clearShader();
}

void Shader::compileShader(const char* vs_source, const char* fs_source) {
    shaderID = glCreateProgram();

    if (!shaderID) {
        throw runtime_error("Failed to create shader program!");
    }

    addShader(shaderID, vs_source, GL_VERTEX_SHADER);
    addShader(shaderID, fs_source, GL_FRAGMENT_SHADER);

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
    uniformProjection = glGetUniformLocation(shaderID, "projection");
}

void Shader::addShader(GLuint program, const char* source, GLenum shaderType) {
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
