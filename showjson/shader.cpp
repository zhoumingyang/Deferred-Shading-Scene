#include "shader.h"
const char* pVSFileNameDefault = "./shader/showjson.vs";
const char* pFSFileNameDefault = "./shader/showjson.fs";

Shader::Shader(const char* _pvsFileName, const char* _pfsFileName) {
	pvsFileName = _pvsFileName;
	pfsFileName = _pfsFileName;
	if (_pvsFileName == NULL) {
		pvsFileName = pVSFileNameDefault;
	}
	if (_pfsFileName == NULL) {
		pfsFileName = pFSFileNameDefault;
	}
}

Shader::Shader() {
	pvsFileName = pVSFileNameDefault;
	pfsFileName = pFSFileNameDefault;
}

Shader::~Shader() {
	pvsFileName = NULL;
	pfsFileName = NULL;
	if (shaderProgram == 0xffffffff) {
		glDeleteProgram(shaderProgram);
	}
	mapUniformLocation.clear();
	std::map<std::string, GLuint>().swap(mapUniformLocation);
}

bool Shader::doRead(std::ifstream& f, std::string& outFile) {
	bool ret = false;
	if (f.is_open()) {
		std::string line;
		while (std::getline(f, line)) {
			outFile.append(line);
			outFile.append("\n");
		}
		f.close();
		ret = true;
	}
	else {
		std::cout << "open file error." << std::endl;
	}
	return ret;
}

bool Shader::readFile(std::string& outVsFile, std::string& outFsFile) {
	std::ifstream f0(pvsFileName);
	std::ifstream f1(pfsFileName);
	bool ret = false;
	if (doRead(f0, outVsFile) && doRead(f1, outFsFile)) {
		ret = true;
	}
	return ret;
}

void Shader::attachShader(const GLuint& ShaderProgram, const char* pShaderText, GLenum ShaderType) {
	GLuint ShaderObj = glCreateShader(ShaderType);
	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(1);
	}
	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	glShaderSource(ShaderObj, 1, p, Lengths);
	glCompileShader(ShaderObj);
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		getchar();
		exit(1);
	}
	glAttachShader(ShaderProgram, ShaderObj);
}

void Shader::compileShader() {
	shaderProgram = glCreateProgram();
	if (shaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		getchar();
		exit(1);
	}
	std::string vs, fs;
	if (!readFile(vs, fs)) {
		exit(1);
	}

	parseUniform(vs);
	parseUniform(fs);

	attachShader(shaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	attachShader(shaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (success == 0) {
		glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		getchar();
		exit(1);
	}

	glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		getchar();
		exit(1);
	}
	glUseProgram(shaderProgram);
	initUniformLocation();
}

GLuint Shader::getShaderObject() const {
	return shaderProgram;
}

void Shader::use() const {
	glUseProgram(shaderProgram);
}

void Shader::unuse() const {
	glUseProgram(0);
}

void Shader::setMeshes(const std::vector<Mesh*>& _meshes) {
	meshes = _meshes;
}

std::vector<Mesh*> Shader::getMeshes() const {
	return meshes;
}


int Shader::getUniformLocation(const GLchar* varName) {
	auto it = mapUniformLocation.find(varName);
	if (it != mapUniformLocation.end()) {
		GLuint location = mapUniformLocation.at(varName);
		return location;
	}
	return glGetUniformLocation(shaderProgram, varName);
}

void Shader::setUniform1i(const GLint& location, const int& value) const {
	if (location != 0xffffffff) {
		glUniform1i(location, value);
	}
	else {
		//std::cout << "location is wrong" << std::endl;
	}
}

void Shader::setUniformMatrix4fv(const GLint& location, const GLfloat* value) const {
	if (location != 0xffffffff) {
		glUniformMatrix4fv(location, 1, GL_FALSE, value);
	}
	else {
		//std::cout << "location is wrong" << std::endl;
	}
}

void Shader::setUniform1f(const GLint& location, const float& value) const {
	if (location != 0xffffffff) {
		glUniform1f(location, value);
	}
	else {
		//std::cout << "location is wrong" << std::endl;
	}
}

void Shader::setUniform3f(const GLint& location, const glm::vec3& value) const {
	if (location != 0xffffffff) {
		glUniform3f(location, value.x, value.y, value.z);
	}
	else {
		//std::cout << "location is wrong" << std::endl;
	}
}

void Shader::parseUniform(const std::string& shaderSource) {
	size_t uniformPos = shaderSource.find("uniform", 0);
	while(uniformPos != std::string::npos) {
		size_t srcEndPos = shaderSource.find(";", uniformPos + 1);
		if (srcEndPos != std::string::npos) {
			std::string uniformDefineStr = shaderSource.substr(uniformPos, srcEndPos - uniformPos + 1);
			if (uniformDefineStr.find("=") == std::string::npos) {
				size_t spacePos = uniformDefineStr.find_last_of(" ");
				srcEndPos = uniformDefineStr.find(";");
				if (spacePos != std::string::npos && srcEndPos != std::string::npos) {
					std::string uniformVar = uniformDefineStr.substr(spacePos + 1, srcEndPos - spacePos - 1);
					GLuint location = 0xffffffff;
					mapUniformLocation.insert(std::map<std::string, GLuint>::value_type(uniformVar, location));
				}
			}
		}
		uniformPos = shaderSource.find("uniform", uniformPos + 1);
	}
}

void Shader::initUniformLocation() {
	std::map<std::string, GLuint>::iterator it;
	for (it = mapUniformLocation.begin(); it != mapUniformLocation.end(); it++) {
		GLuint location = glGetUniformLocation(shaderProgram, it->first.c_str());
		if (location != 0xffffffff) {
			it->second = location;
		}
	}
}

void Shader::setUniform1i(const std::string& varName, const int& value) const {
	auto it = mapUniformLocation.find(varName);
	if (it != mapUniformLocation.end()) {
		GLuint location = mapUniformLocation.at(varName);
		if (location != 0xffffffff) {
			glUniform1i(location, value);
		}
	}
}

void Shader::setUniformMatrix4fv(const std::string& varName, const GLfloat* value) const {
	auto it = mapUniformLocation.find(varName);
	if (it != mapUniformLocation.end()) {
		GLuint location = mapUniformLocation.at(varName);
		if (location != 0xffffffff) {
			glUniformMatrix4fv(location, 1, GL_FALSE, value);
		}
	}
}

void Shader::setUniform1f(const std::string& varName, const float& value) const {
	auto it = mapUniformLocation.find(varName);
	if (it != mapUniformLocation.end()) {
		GLuint location = mapUniformLocation.at(varName);
		if (location != 0xffffffff) {
			glUniform1f(location, value);
		}
	}
}

void Shader::setUniform3f(const std::string& varName, const glm::vec3& value) const {
	auto it = mapUniformLocation.find(varName);
	if (it != mapUniformLocation.end()) {
		GLuint location = mapUniformLocation.at(varName);
		if (location != 0xffffffff) {
			glUniform3f(location, value.x, value.y, value.z);
		}
	}
}