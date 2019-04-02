#pragma once
#ifndef SPHERE_MATH_H
#define SPHERE_MATH_H
#include "geomath.h"
#include <vector>

//reference: https://threejs.org/docs/#api/en/math/Sphere

class Box3Math;
class PlaneMath;

class SphereMath :public GeoMath {
private:
	float radius;
	glm::vec3 center;
	GeoType type;
public:
	SphereMath();
	SphereMath(const glm::vec3& _center, const float& radius);
	~SphereMath();
	float getRadius() const;
	glm::vec3 getCenter() const;
	void setRadius(const float& _radius);
	void setCenter(const glm::vec3& _center);
	void set(const glm::vec3& _center, const float& radius);
	void setFromPosition(const std::vector<float>& position);
	void setFromPosition(const std::vector<glm::vec3>& position);
	bool empty() const;
	bool containsPoint(const glm::vec3& point) const;
	float distanceToPoint(const glm::vec3& point) const;
	bool intersectsSphere(const SphereMath& sphere) const;
	bool intersectsBox(const Box3Math& box) const;
	bool intersectsPlane(const PlaneMath& plane) const;
	glm::vec3 clampPoint(const glm::vec3& point) const;
	Box3Math getBoundingBox() const;
	void applyMat4(glm::mat4 matrix);
	void translate(glm::vec3 offset);
	bool equals(const SphereMath& sphere) const;
	virtual float computeSurfaceArea() const;
};

#endif // !SPHERE_MATH_H
