#pragma once
#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H
#include "light.h"
#include "shader.h"
#define MAX_ATTACH_SHADERS 100

struct PointLightUniformLocation {
	GLuint color;
	GLuint ambient;
	GLuint diffuse;
	GLuint position;
	GLuint constant;
	GLuint linear;
	GLuint exp;
	GLuint shader;
};

class PointLight: public Light {
private:
	glm::vec3 position;
	LightAttenuation attenuation;
	PointLightUniformLocation unifLocations[MAX_ATTACH_SHADERS];
	int shaderCount;
public:
	PointLight();
	PointLight(const glm::vec3 _position, const LightAttenuation& _attenuation);
	PointLight(const glm::vec3 _position, const LightAttenuation& _attenuation, const glm::vec3& _color, const float& _ambient, const float& _diffuse);
	~PointLight();
	void setPosition(const glm::vec3& _position);
	glm::vec3 getPosition() const;
	void setPositionUniform(const GLint& location) const;
	void setAttenuation(const LightAttenuation& _attenuation);
	LightAttenuation getAttenuation() const;
	void setAttenuationUniform(const GLint& consLocation, const  GLint& linrLocation, const GLint expLocation) const;
	void initUniformLocation(Shader& shader, const int& index);
	PointLightUniformLocation getUniformLocation(Shader& shader) const;
	void setAllUniformParams(Shader& shader) const;
};
#endif // !POINT_LIGHT_H
