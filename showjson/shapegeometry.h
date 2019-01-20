#pragma once
#ifndef SHAPE_GEOMETRY_H
#define SHAPE_GEOMETRY_H
#include <iostream>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include "glm/glm.hpp"
#pragma comment(lib,"glew32.lib")

class ShapeGeometry {
private:
	std::string shapeName;
	std::vector<glm::vec3> pos;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<int> indices;

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec4 rot;

	glm::vec3 color;
public:
	ShapeGeometry();
	~ShapeGeometry();
	void clear();

	std::string getShapeName() const;
	void setShapeName(const std::string& shape);

	std::vector<glm::vec3> getVertices() const;
	void setVertices(const std::vector<glm::vec3>& _pos);

	std::vector<glm::vec2> getUvs() const;
	void setUvs(const std::vector<glm::vec2>& _uvs);

	std::vector<glm::vec3> getNormals() const;
	void setNormals(const std::vector<glm::vec3>& _normals);

	std::vector<int> getIndices() const;
	void setIndices(const std::vector<int>& _indices);

	void pushVertex(const float& x, const float& y, const float& z);
	void pushVertex(const glm::vec3& ver);

	void pushUv(const float& u, const float& v);
	void pushUv(const glm::vec2& uv);

	void pushNormal(const float& x, const float& y, const float& z);
	void pushNormal(const glm::vec3& normal);

	void addIndex(unsigned int i1, unsigned int i2, unsigned int i3);

	int getVerticesCount() const;
	int getUvsCount() const;
	int getNormalsCount() const;
	int getIndicesCount() const;

	void setColor(const float& x, const float& y, const float& z);
	void setColor(const glm::vec3& _color);

	void setPosition(const float& x, const float& y, const float& z);
	void setPosition(const glm::vec3& _position);

	void setScale(const float& x, const float& y, const float& z);
	void setScale(const glm::vec3& _scale);

	void setRot(const float& x, const float& y, const float& z, const float& w);
	void setRot(const glm::vec4& _rot);

	glm::vec3 getPosition() const;
	glm::vec3 getScale() const;
	glm::vec4 getRot() const;
	glm::vec3 getColor() const;

	glm::mat4 getModelMatrix() const;
	glm::mat4 getNormalMatrix() const;

	virtual void draw() {

	}
};
#endif // !SHAPE_GEOMETRY_H
