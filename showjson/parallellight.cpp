#include "parallellight.h"

Parallellight::Parallellight() :Light() {
	direction = glm::vec3(0.0, 0.0, 0.0);
	lightName = "paralle light";
	shaderCount = 0;
}

Parallellight::Parallellight(const glm::vec3& _color, const float& _ambient,
	const float& _diffuse, const glm::vec3& _direction) : Light(_color, _ambient, _diffuse) {
	direction = _direction;
	lightName = "paralle light";
	shaderCount = 0;
}

Parallellight::~Parallellight() {

}

void Parallellight::setDirection(const glm::vec3& _direction) {
	direction = _direction;
}

glm::vec3 Parallellight::getDirection() const {
	return direction;
}

void Parallellight::setDirectionUniform(const GLint& location) const {
	if (location != 0xffffffff) {
		glUniform3f(location, direction.x, direction.y, direction.z);
	}
	else {
		//std::cout << "location error" << std::endl;
	}
}

void Parallellight::initUniformLocation(Shader& shader, const int& index) {
	PrleLightUniformLocation unifLocation;
	std::string pt = "prleLights";
	pt += ("[" + std::to_string(index) + "]");
	unifLocation.color = shader.getUniformLocation((pt + ".color").c_str());
	unifLocation.ambient = shader.getUniformLocation((pt + ".ambient").c_str());
	unifLocation.diffuse = shader.getUniformLocation((pt + ".diffuse").c_str());
	unifLocation.direction = shader.getUniformLocation((pt + ".direction").c_str());
	unifLocation.shader = shader.getShaderObject();
	unifLocations[shaderCount++] = unifLocation;
	if (shaderCount >= MAX_ATTACH_SHADERS) {
		shaderCount = MAX_ATTACH_SHADERS - 1;
	}
}

PrleLightUniformLocation Parallellight::getUniformLocation(Shader& shader) const {
	PrleLightUniformLocation unifLocation;
	GLuint shaderObj = shader.getShaderObject();
	for (int i = 0; i < shaderCount; i++) {
		if (unifLocations[i].shader == shaderObj) {
			unifLocation = unifLocations[i];
		}
	}
	return unifLocation;
}

void Parallellight::setAllUniformParams(Shader& shader) const {
	PrleLightUniformLocation unifLocation = getUniformLocation(shader);
	setColorUniform(unifLocation.color);
	setAmbientUniform(unifLocation.ambient);
	setDiffuseUniform(unifLocation.diffuse);
	setDirectionUniform(unifLocation.direction);
}