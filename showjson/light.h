#pragma once
#ifndef LIGHT_H
#define LIGHT_H
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include "glm.hpp"
#pragma comment(lib,"glew32.lib")

class Light {
public:
	Light();
	Light(const glm::vec3& _color, const float& _ambient, const float& _diffuse);
	virtual ~Light();
	float ambient;
	float diffuse;
	glm::vec3 color;
	std::string lightName;
	void setAmbient(const float& _ambient);
	void setDiffuse(const float& _diffuse);
	void setColor(const glm::vec3& _color);
	float getAmbient() const;
	float getDiffuse() const;
	glm::vec3 getColor() const;
	std::string getLightName() const;
	void setColorUniform(const GLint& location) const;
	void setAmbientUniform(const GLint& location) const;
	void setDiffuseUniform(const GLint& location) const;
};

struct LightAttenuation {
	float constant;
	float linear;
	float exp;
	LightAttenuation() {
		constant = 1.0f;
		linear = 0.0f;
		exp = 0.0f;
	}
	LightAttenuation(const float& _constant, const float& _linear, const float& _exp) {
		constant = _constant;
		linear = _linear;
		exp = _exp;
	}
};
#endif // !LIGHT_H