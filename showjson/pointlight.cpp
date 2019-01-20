#include "pointlight.h"

PointLight::PointLight() :Light() {
	position = glm::vec3(0.0, 0.0, 0.0);
	attenuation = LightAttenuation();
	lightName = "point light";
	shaderCount = 0;
}

PointLight::PointLight(const glm::vec3 _position, const LightAttenuation& _attenuation) : Light() {
	position = _position;
	attenuation = _attenuation;
	lightName = "point light";
	shaderCount = 0;
}

PointLight::PointLight(const glm::vec3 _position, const LightAttenuation& _attenuation, const glm::vec3& _color, const float& _ambient, const float& _diffuse)
	: Light(_color, _ambient, _diffuse) {
	position = _position;
	attenuation = _attenuation;
	shaderCount = 0;
}

PointLight::~PointLight() {

}

void PointLight::setPosition(const glm::vec3& _position) {
	position = _position;
}

glm::vec3 PointLight::getPosition() const {
	return position;
}

void PointLight::setPositionUniform(const GLint& location) const {
	glUniform3f(location, position.x, position.y, position.z);
}

void PointLight::setAttenuation(const LightAttenuation& _attenuation) {
	attenuation = _attenuation;
}

LightAttenuation PointLight::getAttenuation() const {
	return attenuation;
}

void PointLight::setAttenuationUniform(const GLint& consLocation, const  GLint& linrLocation, const GLint expLocation) const {
	glUniform1f(consLocation, attenuation.constant);
	glUniform1f(linrLocation, attenuation.linear);
	glUniform1f(expLocation, attenuation.exp);
}

void PointLight::initUniformLocation(Shader& shader, const int& index) {
	PointLightUniformLocation unifLocation;
	std::string pt = "pointLights";
	pt += ("[" + std::to_string(index) + "]");
	unifLocation.color = shader.getUniformLocation((pt + ".color").c_str());
	unifLocation.ambient = shader.getUniformLocation((pt + ".ambient").c_str());
	unifLocation.diffuse = shader.getUniformLocation((pt + ".diffuse").c_str());
	unifLocation.position = shader.getUniformLocation((pt + ".position").c_str());
	unifLocation.constant = shader.getUniformLocation((pt + ".attenuation.constant").c_str());
	unifLocation.linear = shader.getUniformLocation((pt + ".attenuation.linear").c_str());
	unifLocation.exp = shader.getUniformLocation((pt + ".attenuation.exp").c_str());
	unifLocation.shader = shader.getShaderObject();
	unifLocations[shaderCount++] = unifLocation;
	if (shaderCount >= MAX_ATTACH_SHADERS) {
		shaderCount = MAX_ATTACH_SHADERS - 1;
	}
}

PointLightUniformLocation PointLight::getUniformLocation(Shader& shader) const {
	PointLightUniformLocation unifLocation;
	GLuint shaderObj = shader.getShaderObject();
	for (int i = 0; i < shaderCount; i++) {
		if (unifLocations[i].shader == shaderObj) {
			unifLocation = unifLocations[i];
		}
	}
	return unifLocation;
}

void PointLight::setAllUniformParams(Shader& shader) const {
	PointLightUniformLocation unifLocation = getUniformLocation(shader);
	setColorUniform(unifLocation.color);
	setAmbientUniform(unifLocation.ambient);
	setDiffuseUniform(unifLocation.diffuse);
	setPositionUniform(unifLocation.position);
	setAttenuationUniform(unifLocation.constant, unifLocation.linear, unifLocation.exp);
}