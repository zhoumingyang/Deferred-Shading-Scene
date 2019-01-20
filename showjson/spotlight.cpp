#include "spotlight.h"

SpotLight::SpotLight() :Light() {
	direction = glm::vec3(0.0, 0.0, 0.0);
	position = glm::vec3(0.0, 0.0, 0.0);
	cutoff = 0.0;
	attenuation = LightAttenuation();
	lightName = "spot light";
	shaderCount = 0;
}

SpotLight::SpotLight(const glm::vec3& _position, const glm::vec3& _direction,
	const float& _cutoff, const LightAttenuation& _attenuation) :Light() {
	position = _position;
	direction = _direction;
	cutoff = _cutoff;
	attenuation = _attenuation;
	lightName = "spot light";
	shaderCount = 0;
}

SpotLight::SpotLight(const glm::vec3& _position, const glm::vec3& _direction,
	const float& _cutoff, const LightAttenuation& _attenuation,
	const glm::vec3& _color, const float& _ambient, const float& _diffuse) :Light(_color, _ambient, _diffuse) {
	position = _position;
	direction = _direction;
	cutoff = _cutoff;
	attenuation = _attenuation;
	shaderCount = 0;
}

SpotLight::~SpotLight() {

}

glm::vec3 SpotLight::getDirection() const {
	return direction;
}

glm::vec3 SpotLight::getPosition() const {
	return position;
}

float SpotLight::getCutoff() const {
	return cutoff;
}

LightAttenuation SpotLight::getAttenuation() const {
	return attenuation;
}

void SpotLight::setDirection(const glm::vec3& _direciton) {
	direction = _direciton;
}

void SpotLight::setPosition(const glm::vec3& _position) {
	position = _position;
}

void SpotLight::setCutoff(const float& _cutoff) {
	cutoff = _cutoff;
}

void SpotLight::setAttenuation(const LightAttenuation& _attenuation) {
	attenuation = _attenuation;
}

void SpotLight::setPositionUniform(const GLint& location) const {
	if (location != 0xffffff) {
		glUniform3f(location, position.x, position.y, position.z);
	}
	else {
		//std::cout << "location error" << std::endl;
	}
}

void SpotLight::setDirecitonUniform(const GLint& location) const {
	if (location != 0xffffff) {
		glUniform3f(location, direction.x, direction.y, direction.z);
	}
	else {
		//std::cout << "location error" << std::endl;
	}
}

void SpotLight::setCutoffUniform(const GLint& location) const {
	if (location != 0xffffff) {
		glUniform1f(location, cutoff);
	}
	else {
		//std::cout << "location error" << std::endl;
	}
}

void SpotLight::setAttenuationUniform(const GLint& consLocation, const  GLint& linrLocation, const GLint expLocation) const {
	glUniform1f(consLocation, attenuation.constant);
	glUniform1f(linrLocation, attenuation.linear);
	glUniform1f(expLocation, attenuation.exp);
}

void SpotLight::initUniformLocation(Shader& shader, const int& index) {
	SpotLightUniformLocation unifLocation;
	std::string pt = "spotLights";
	pt += ("[" + std::to_string(index) + "]");
	unifLocation.color = shader.getUniformLocation((pt + ".color").c_str());
	unifLocation.ambient = shader.getUniformLocation((pt + ".ambient").c_str());
	unifLocation.diffuse = shader.getUniformLocation((pt + ".diffuse").c_str());
	unifLocation.position = shader.getUniformLocation((pt + ".position").c_str());
	unifLocation.cutoff = shader.getUniformLocation((pt + ".cutoff").c_str());
	unifLocation.direction = shader.getUniformLocation((pt + ".direction").c_str());
	unifLocation.constant = shader.getUniformLocation((pt + ".attenuation.constant").c_str());
	unifLocation.linear = shader.getUniformLocation((pt + ".attenuation.linear").c_str());
	unifLocation.exp = shader.getUniformLocation((pt + ".attenuation.exp").c_str());
	unifLocation.shader = shader.getShaderObject();
	unifLocations[shaderCount++] = unifLocation;
}

SpotLightUniformLocation SpotLight::getUniformLocation(Shader& shader) const {
	SpotLightUniformLocation unifLocation;
	GLuint shaderObj = shader.getShaderObject();
	for (int i = 0; i < shaderCount; i++) {
		if (unifLocations[i].shader == shaderObj) {
			unifLocation = unifLocations[i];
		}
	}
	return unifLocation;
}

void SpotLight::setAllUniformParams(Shader& shader) const {
	SpotLightUniformLocation unifLocation = getUniformLocation(shader);
	setColorUniform(unifLocation.color);
	setAmbientUniform(unifLocation.ambient);
	setDiffuseUniform(unifLocation.diffuse);
	setPositionUniform(unifLocation.position);
	setDirecitonUniform(unifLocation.direction);
	setCutoffUniform(unifLocation.cutoff);
	setAttenuationUniform(unifLocation.constant, unifLocation.linear, unifLocation.exp);
}