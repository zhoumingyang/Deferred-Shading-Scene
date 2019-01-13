#include "parallellight.h"

Parallellight::Parallellight() :Light() {
	direction = glm::vec3(0.0, 0.0, 0.0);
	lightName = "paralle light";
}

Parallellight::Parallellight(const glm::vec3& _color, const float& _ambient,
	const float& _diffuse, const glm::vec3& _direction) : Light(_color, _ambient, _diffuse) {
	direction = _direction;
	lightName = "paralle light";
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
	std::string pt = "prleLights";
	pt += ("[" + std::to_string(index) + "]");
	unifLocation.color = shader.getUniformLocation((pt + ".color").c_str());
	unifLocation.ambient = shader.getUniformLocation((pt + ".ambient").c_str());
	unifLocation.diffuse = shader.getUniformLocation((pt + ".diffuse").c_str());
	unifLocation.direction = shader.getUniformLocation((pt + ".direction").c_str());
}

PrleLightUniformLocation Parallellight::getUniformLocation() const {
	return unifLocation;
}

void Parallellight::setAllUniformParams() const {
	setColorUniform(unifLocation.color);
	setAmbientUniform(unifLocation.ambient);
	setDiffuseUniform(unifLocation.diffuse);
	setDirectionUniform(unifLocation.direction);
}