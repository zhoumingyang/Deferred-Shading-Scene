#pragma once
#ifndef BOX3_MATH_H
#define BOX3_MATH_H
#include "geomath.h"
#include <vector>
#include <limits>

//reference: https://threejs.org/docs/index.html#api/en/math/Box3

class Mesh;
class SphereMath;
class PlaneMath;
class TriangleMath;

class Box3Math :public GeoMath {
private:
	GeoType type;
	glm::vec3 min;
	glm::vec3 max;
	bool satForAxes(const std::vector<float>& axes, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& extents) const;
public:
	Box3Math();
	Box3Math(const glm::vec3& _min, const glm::vec3& _max);
	~Box3Math();
	glm::vec3 getMin() const;
	glm::vec3 getMax() const;
	void setMin(const glm::vec3& _min);
	void setMax(const glm::vec3& _max);
	void set(const glm::vec3& _min, const glm::vec3& _max);
	void setFromMesh(const Mesh& mesh);
	void setFromPosition(const std::vector<float>& position);
	void setFromPosition(const std::vector<glm::vec3>& position);
	void setFromCenterAndSize(const glm::vec3& center, const glm::vec3& size);
	void makeEmpty();
	bool isEmpty() const;
	glm::vec3 getCenter() const;
	glm::vec3 getSize() const;
	void expandByPoint(const glm::vec3& point);
	void expandByVector(const glm::vec3& vector);
	void expandByScalar(const float& scalar);
	bool containsPoint(const glm::vec3& point) const;
	bool containsBox(const Box3Math& box) const;
	bool intersectsBox(const Box3Math& box) const;
	bool intersectsSphere(const SphereMath& sphere) const;
	bool intersectsPlane(const PlaneMath& plane) const;
	bool intersectsTriangle(const TriangleMath& plane) const;
	glm::vec3 clampPoint(const glm::vec3& point) const;
	float distanceToPoint(const glm::vec3& point) const;
	SphereMath getBoundingSphere() const;
	Box3Math intersect(const Box3Math& box);
	Box3Math unionBox(const Box3Math& box);
	void applyMat4(glm::mat4 matrix);
	void translate(glm::vec3 offset);
	bool equals(const Box3Math& box) const;
	virtual float computeSurfaceArea() const;
};

#endif // !BOX3_MATH_H