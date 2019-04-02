#pragma once
#ifndef RAY_H
#define RAY_H
#include <iostream>
#include "glm.hpp"

//reference: https://threejs.org/docs/#api/en/math/Ray

class Box3Math;
class SphereMath;
class PlaneMath;
class TriangleMath;

class Ray {
private:
	glm::vec3 origin;
	glm::vec3 direction;
public:
	Ray();
	Ray(const glm::vec3& _origin, const glm::vec3& _direction);
	~Ray();
	void set(const glm::vec3& _origin, const glm::vec3& _direction);
	void setOrigin(const glm::vec3& _origin);
	void setDirection(const glm::vec3& _direction);
	glm::vec3 getOrigin() const;
	glm::vec3 getDirection() const;
	glm::vec3 at(const float& t) const;
	glm::vec3 lookat(const glm::vec3& v);
	Ray recast(const float& t);
	glm::vec3 closestPointToPoint(const glm::vec3& point) const;
	float distanceToPoint(const glm::vec3& point) const;
	float distanceSqToPoint(const glm::vec3& point) const;
	//float distanceSqToSegment()
	glm::vec3 intersectBox(const Box3Math& box3) const;
	bool intersectsBox(const Box3Math& box3) const;

	glm::vec3 intersectSphere(const SphereMath& sphere) const;
	bool intersectsSphere(const SphereMath& sphere) const;

	float distanceToPlane(const PlaneMath& plane) const;
	glm::vec3 intersectPlane(const PlaneMath& plane) const;
	bool intersectsPlane(const PlaneMath& plane) const;

	glm::vec3 intersectTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3&c, const bool& backfaceCulling) const;
	bool equals(const Ray& ray) const;
	void applyMat4(const glm::mat4& mat);
};

#endif // !RAY_H