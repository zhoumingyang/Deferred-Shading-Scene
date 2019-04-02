#pragma once
#ifndef SHADER_H
#define SHADER_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
//#include <GL/glew.h>
//#include <GL/glut.h>
#include "GL/glew.h"
#include "GL/glut.h"
#include "glm.hpp"
#pragma comment(lib,"glew32.lib")
class Mesh;
class Shader {
private:
	const char* pvsFileName;
	const char* pfsFileName;
	const char* pgsFileName;
	GLuint shaderProgram;
	bool readFile(std::string& outVsFile, std::string& outFsFile);
	bool readFile(std::string& outVsFile, std::string& outFsFile, std::string& outGsFile);
	bool doRead(std::ifstream& f, std::string& outFile);
	std::vector<Mesh*> meshes;
	std::map<std::string, GLuint> mapUniformLocation;
	void parseUniform(const std::string& shaderSource);
	void initUniformLocation();
public:
	Shader(const char* _pvsFileName, const char* _pfsFileName);
	Shader(const char* _pvsFileName, const char* _pfsFileName, const char* _pgsFileName);
	Shader();
	~Shader();
	void attachShader(const GLuint& ShaderProgram, const char* pShaderText, GLenum ShaderType);
	void compileShader();
	GLuint getShaderObject() const;
	void use() const;
	void unuse() const;
	void setMeshes(const std::vector<Mesh*>& _meshes);
	std::vector<Mesh*> getMeshes() const;
	int getUniformLocation(const GLchar* varName);
	void setUniform1i(const GLint& location, const int& value) const ;
	void setUniformMatrix4fv(const GLint& location, const GLfloat* value) const ;
	void setUniform1f(const GLint& location, const float& value) const;
	void setUniform3f(const GLint& location, const glm::vec3& value) const;
	void setUniform1i(const std::string& varName, const int& value) const;
	void setUniformMatrix4fv(const std::string& varName, const GLfloat* value) const;
	void setUniform1f(const std::string& varName, const float& value) const;
	void setUniform3f(const std::string& varName, const glm::vec3& value) const;

};
#endif // !SHADER_H