#pragma once
#ifndef PLANE_MATH_H
#define PLANE_MATH_H
#include "geomath.h"
#include <vector>

//reference: https://threejs.org/docs/#api/en/math/Plane

class Box3Math;
class SphereMath;
//class Line3Math;

class PlaneMath :public GeoMath {
private:
	GeoType type;
	glm::vec3 normal;
	float constant;
public:
	PlaneMath();
	PlaneMath(const glm::vec3& _normal, const float& _constant);
	~PlaneMath();
	glm::vec3 getNormal() const;
	float getConstant() const;
	void setNormal(const glm::vec3& _normal);
	void setConstant(const float& _constant);
	void set(const glm::vec3& _normal, const float& _constant);
	void setComponents(const float& x, const float& y, const float& z, const float& c);
	void setFromNormalAndCoplanarPoint(const glm::vec3& _normal, const glm::vec3& _point);
	void setFromCoplanarPoints(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
	void normalize();
	void negate();
	float distanceToPoint(const glm::vec3& point) const;
	float distanceToSphere(const SphereMath& sphere) const;
	glm::vec3 projectPoint(const glm::vec3& point) const;
	bool intersectsBox(const Box3Math& box) const;
	bool intersectsSphere(const SphereMath&  sphere) const;
	glm::vec3 coplanarPoint() const;
	void translate(const glm::vec3& offset);
	void applyMat4(glm::mat4 matrix, glm::mat3 normalMatrix);
	bool equals(const PlaneMath& plane) const;
	virtual float computeSurfaceArea() const;
};

#endif // !PLANE_MATH_H