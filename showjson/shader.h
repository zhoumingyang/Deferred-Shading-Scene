#pragma once
#ifndef SHADER_H
#define SHADER_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include "glm.hpp"
#pragma comment(lib,"glew32.lib")
class Mesh;
class Shader {
private:
	const char* pvsFileName;
	const char* pfsFileName;
	GLuint shaderProgram;
	bool readFile(std::string& outVsFile, std::string& outFsFile);
	bool doRead(std::ifstream& f, std::string& outFile);
	std::vector<Mesh*> meshes;
public:
	Shader(const char* _pvsFileName, const char* _pfsFileName);
	Shader();
	~Shader();
	void attachShader(const GLuint& ShaderProgram, const char* pShaderText, GLenum ShaderType);
	void compileShader();
	GLuint getShaderObject() const;
	void use() const;
	void unuse() const;
	int getUniformLocation(const GLchar* varName);
	void setUniform1i(const GLint& location, const int& value);
	void setUniformMatrix4fv(const GLint& location, const GLfloat* value);
	void setMeshes(const std::vector<Mesh*>& _meshes);
	std::vector<Mesh*> getMeshes() const;
	void setUniform1f(const GLint& location, const float& value) const;
	void setUniform3f(const GLint& location, const glm::vec3& value) const;
};
#endif // !SHADER_H