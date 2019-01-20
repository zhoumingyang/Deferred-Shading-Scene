#pragma once
#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H
#include "light.h"
#include "shader.h"
#define MAX_ATTACH_SHADERS 100

struct SpotLightUniformLocation {
	GLuint color;
	GLuint ambient;
	GLuint diffuse;
	GLuint position;
	GLuint cutoff;
	GLuint direction;
	GLuint constant;
	GLuint linear;
	GLuint exp;
	GLuint shader;
};

class SpotLight :public Light {
private:
	glm::vec3 direction;
	glm::vec3 position;
	float cutoff;
	LightAttenuation attenuation;
	SpotLightUniformLocation unifLocations[MAX_ATTACH_SHADERS];
	int shaderCount;
public:
	SpotLight();
	SpotLight(const glm::vec3& _position, const glm::vec3& _direction,
		const float& _cutoff, const LightAttenuation& _attenuation);
	SpotLight(const glm::vec3& _position, const glm::vec3& _direction,
		const float& _cutoff, const LightAttenuation& _attenuation,
		const glm::vec3& _color, const float& _ambient, const float& _diffuse);
	~SpotLight();
	glm::vec3 getDirection() const;
	glm::vec3 getPosition() const;
	float getCutoff() const;
	LightAttenuation getAttenuation() const;
	void setDirection(const glm::vec3& _direciton);
	void setPosition(const glm::vec3& _position);
	void setCutoff(const float& _cutoff);
	void setAttenuation(const LightAttenuation& _attenuation);
	void setPositionUniform(const GLint& location) const;
	void setDirecitonUniform(const GLint& location) const;
	void setCutoffUniform(const GLint& location) const;
	void setAttenuationUniform(const GLint& consLocation, const  GLint& linrLocation, const GLint expLocation) const;
	void initUniformLocation(Shader& shader, const int& index);
	SpotLightUniformLocation getUniformLocation(Shader& shader) const;
	void setAllUniformParams(Shader& shader) const;
};
#endif // !SPOT_LIGHT_H