#pragma once
#ifndef PARALLEL_LIGHT_H
#define PARALLEL_LIGHT_H
#include "light.h"
#include "shader.h"

struct PrleLightUniformLocation {
	GLuint color;
	GLuint ambient;
	GLuint diffuse;
	GLuint direction;
};

class Parallellight :public Light {
private:
	glm::vec3 direction;
	PrleLightUniformLocation unifLocation;
public:
	Parallellight();
	Parallellight(const glm::vec3& _color, const float& _ambient, const float& _diffuse, const glm::vec3& _direction);
	virtual ~Parallellight();
	void setDirection(const glm::vec3& _direction);
	glm::vec3 getDirection() const;
	void setDirectionUniform(const GLint& location) const;
	void initUniformLocation(Shader& shader, const int& index);
	PrleLightUniformLocation getUniformLocation() const;
	void setAllUniformParams() const;
};
#endif // !PARALLEL_LIGHT_H