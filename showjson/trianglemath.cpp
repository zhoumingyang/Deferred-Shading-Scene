#include "trianglemath.h"
#include "box3math.h"
#include "planemath.h"

TriangleMath::TriangleMath() {
	a = glm::vec3(0.0, 0.0, 0.0);
	b = glm::vec3(0.0, 0.0, 0.0);
	c = glm::vec3(0.0, 0.0, 0.0);
}

TriangleMath::TriangleMath(const glm::vec3& _a, const glm::vec3& _b, const glm::vec3& _c) {
	a = _a;
	b = _b;
	c = _c;
}

TriangleMath::~TriangleMath() {

}

glm::vec3 TriangleMath::_a() const {
	return a;
}

glm::vec3 TriangleMath::_b() const {
	return b;
}

glm::vec3 TriangleMath::_c() const {
	return c;
}

void TriangleMath::seta(const glm::vec3& _a) {
	a = _a;
}

void TriangleMath::setb(const glm::vec3& _b) {
	b = _b;
}


void TriangleMath::setc(const glm::vec3& _c) {
	c = _c;
}

void TriangleMath::set(const glm::vec3& _a, const glm::vec3& _b, const glm::vec3& _c) {
	a = _a;
	b = _b;
	c = _c;
}

void TriangleMath::setFromPointsAndIndices(const std::vector<glm::vec3>& points, const int& i0, const int& i1, const int& i2) {
	a = points[i0];
	b = points[i1];
	c = points[i2];
}

glm::vec3 TriangleMath::getNormal() const {
	glm::vec3 target = c - b;
	glm::vec3 v0 = a - b;
	target = glm::cross(target, v0);
	float targetLengthSq = target.x * target.x + target.y * target.y + target.z * target.z;
	if (targetLengthSq > 0) {
		return target * float(1.0 / targetLengthSq);
	}
	return glm::vec3(0.0, 0.0, 0.0);
}

glm::vec3 TriangleMath::getBarycoord(glm::vec3 point) const {
	glm::vec3 target;
	glm::vec3 v0 = c - a;
	glm::vec3 v1 = b - a;
	glm::vec3 v2 = point - a;

	float dot00 = glm::dot(v0, v0);
	float dot01 = glm::dot(v0, v1);
	float dot02 = glm::dot(v0, v2);
	float dot11 = glm::dot(v1, v1);
	float dot12 = glm::dot(v1, v2);

	float denom = dot00 * dot11 - dot01 * dot01;
	if (denom == 0) {
		return glm::vec3(-2, -1, -1);
	}

	float invDenom = 1 / denom;
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
	target = glm::vec3(1 - u - v, v, u);
	return target;
}

bool TriangleMath::containsPoint(glm::vec3 point) const {
	glm::vec3 v1 = getBarycoord(point);
	return (v1.x >= 0) && (v1.y >= 0) && ((v1.x + v1.y) <= 1);
}

glm::vec3 TriangleMath::getUV() const {
	//TODO
	return glm::vec3();
}

glm::vec3 TriangleMath::getMidpoint() const {
	glm::vec3 target = a + b;
	target += c;
	target *= (1.0 / 3.0);
	return target;
}

PlaneMath TriangleMath::getPlane() const {
	PlaneMath plane;
	plane.setFromCoplanarPoints(a, b, c);
	return plane;
}

bool TriangleMath::intersectsBox(const Box3Math& box) const {
	return box.intersectsTriangle(*this);
}

glm::vec3 TriangleMath::closestPointToPoint(const glm::vec3 p) const {
	glm::vec3 target;
	glm::vec3 vab = b - a;
	glm::vec3 vac = c - a;
	glm::vec3 vap = p - a;
	float d1 = glm::dot(vab, vap);
	float d2 = glm::dot(vac, vap);
	if (d1 <= 0 && d2 <= 0) {
		target = a;
		return target;
	}

	glm::vec3 vbp = p - b;
	float d3 = glm::dot(vab, vbp);
	float d4 = glm::dot(vac, vbp);
	if (d3 >= 0 && d4 <= d3) {
		target = b;
		return target;
	}

	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0 && d1 >= 0 && d3 <= 0) {
		float v = d1 / (d1 - d3);
		target = a;
		return target + glm::vec3(vab.x*v, vab.y*v, vab.z*v);
	}

	glm::vec3 vcp = p - c;
	float d5 = glm::dot(vab, vcp);
	float d6 = glm::dot(vac, vcp);
	if (d6 >= 0 && d5 <= d6) {
		target = c;
		return target;
	}

	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0 && d2 >= 0 && d6 <= 0) {
		float w = d2 / (d2 - d6);
		target = a;
		return target + glm::vec3(vac.x*w, vac.y*w, vac.z*w);
	}

	float va = d3 * d6 - d5 * d4;
	if (va <= 0 && (d4 - d3) >= 0 && (d5 - d6) >= 0) {
		glm::vec3 vbc = c - b;
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		target = b;
		return target + glm::vec3(vbc.x*w, vbc.y*w, vbc.z*w);
	}

	float denom = 1.0 / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	target = a;
	target += glm::vec3(vab.x * v, vab.y * v, vab.z * v);
	target += glm::vec3(vac.x * w, vac.y * w, vac.z * w);
	return target;
}

bool TriangleMath::equals(const TriangleMath triangle) const {
	return (a == triangle._a() && b == triangle._b() && c == triangle._c());
}

float TriangleMath::computeSurfaceArea() const {
	glm::vec3 v0 = c - b;
	glm::vec3 v1 = a - b;
	return glm::length(glm::cross(v0, v1)) * 0.5;
}