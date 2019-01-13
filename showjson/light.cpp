#include "light.h"

Light::Light() {
	ambient = 0.0;
	diffuse = 0.0;
	color = glm::vec3(0.0, 0.0, 0.0);
	lightName = "";
}

Light::Light(const glm::vec3& _color, const float& _ambient, const float& _diffuse) {
	color = _color;
	ambient = _ambient;
	diffuse = _diffuse;
	lightName = "";
}

Light::~Light() {

}

void Light::setAmbient(const float& _ambient) {
	ambient = _ambient;
}

void Light::setColor(const glm::vec3& _color) {
	color = _color;
}

void Light::setDiffuse(const float& _diffuse) {
	diffuse = _diffuse;
}

float Light::getAmbient() const {
	return ambient;
}

float Light::getDiffuse() const {
	return diffuse;
}

glm::vec3 Light::getColor() const {
	return color;
}

std::string Light::getLightName() const {
	return lightName;
}

void Light::setColorUniform(const GLint& location) const {
	if (location != 0xffffffff) {
		glUniform3f(location, color.x, color.y, color.z);
	}
	else {
		//std::cout << "location error" << std::endl;
	}
}

void Light::setDiffuseUniform(const GLint& location) const {
	if (location != 0xffffffff) {
		glUniform1f(location, diffuse);
	}
	else {
		//std::cout << "location error" << std::endl;
	}
}

void Light::setAmbientUniform(const GLint& location) const {
	if (location != 0xffffffff) {
		glUniform1f(location, ambient);
	}
	else {
		//std::cout << "location error" << std::endl;
	}
}