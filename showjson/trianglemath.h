#pragma once
#ifndef TRIANGLE_MATH_H
#define TRIANGLE_MATH_H
#include "geomath.h"
#include <vector>

//reference: https://threejs.org/docs/index.html#api/en/math/Triangle

class Box3Math;
class SphereMath;
class PlaneMath;

class TriangleMath :public GeoMath {
private:
	glm::vec3 a, b, c;

public:
	TriangleMath();
	TriangleMath(const glm::vec3& _a, const glm::vec3& _b, const glm::vec3& _c);
	~TriangleMath();
	glm::vec3 _a() const;
	glm::vec3 _b() const;
	glm::vec3 _c() const;
	void seta(const glm::vec3& _a);
	void setb(const glm::vec3& _b);
	void setc(const glm::vec3& _c);
	void set(const glm::vec3& _a, const glm::vec3& _b, const glm::vec3& _c);
	void setFromPointsAndIndices(const std::vector<glm::vec3>& points, const int& i0, const int& i1, const int& i2);
	glm::vec3 getNormal() const;
	glm::vec3 getBarycoord(glm::vec3 point) const;
	bool containsPoint(glm::vec3 point) const;
	glm::vec3 getUV() const;
	glm::vec3 getMidpoint() const;
	PlaneMath getPlane() const;
	bool intersectsBox(const Box3Math& box) const;
	glm::vec3 closestPointToPoint(const glm::vec3 point) const;
	bool equals(const TriangleMath triangle) const;
	virtual float computeSurfaceArea() const;
};


#endif // !TRIANGLE_MATH_H