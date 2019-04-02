#include "planemath.h"
#include "spheremath.h"
#include "box3math.h"

PlaneMath::PlaneMath() {
	constant = 0.0;
	normal = glm::vec3(0.0, 0.0, 0.0);
	type = plane;
}

PlaneMath::PlaneMath(const glm::vec3& _normal, const float& _constant) {
	constant = _constant;
	normal = _normal;
	type = plane;
}

PlaneMath::~PlaneMath() {

}

glm::vec3 PlaneMath::getNormal() const {
	return normal;
}

float PlaneMath::getConstant() const {
	return constant;
}

void PlaneMath::setNormal(const glm::vec3& _normal) {
	normal = _normal;
}

void PlaneMath::setConstant(const float& _constant) {
	constant = _constant;
}

void PlaneMath::set(const glm::vec3& _normal, const float& _constant) {
	normal = _normal;
	constant = _constant;
}

void PlaneMath::setComponents(const float& x, const float& y, const float& z, const float& c) {
	normal = glm::vec3(x, y, z);
	constant = c;
}

void PlaneMath::setFromNormalAndCoplanarPoint(const glm::vec3& _normal, const glm::vec3& _point) {
	normal = _normal;
	constant = -glm::dot(_point, normal);
}

void PlaneMath::setFromCoplanarPoints(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
	glm::vec3 v1;
	glm::vec3 v2;
	normal = glm::normalize(glm::cross(c - b, a - b));
	setFromNormalAndCoplanarPoint(normal, a);
}

void PlaneMath::normalize() {
	float inverseNormalLength = 1.0 / glm::length(normal);
	normal *= inverseNormalLength;
	constant *= inverseNormalLength;
}

void PlaneMath::negate() {
	constant *= -1;
	normal *= -1;
}

float PlaneMath::distanceToPoint(const glm::vec3& point) const {
	return glm::dot(normal, point) + constant;
}

float PlaneMath::distanceToSphere(const SphereMath& sphere) const {
	return distanceToPoint(sphere.getCenter()) - sphere.getRadius();
}

glm::vec3 PlaneMath::projectPoint(const glm::vec3& point) const {
	glm::vec3 target = normal;
	float dis = distanceToPoint(point);
	target *= dis;
	return target + point;
}

bool PlaneMath::intersectsBox(const Box3Math& box) const {
	return box.intersectsPlane(*this);
}

bool PlaneMath::intersectsSphere(const SphereMath&  sphere) const {
	return sphere.intersectsPlane(*this);
}

glm::vec3 PlaneMath::coplanarPoint() const {
	glm::vec3 target = normal;
	return normal * constant;
}

void PlaneMath::translate(const glm::vec3& offset) {
	constant -= glm::dot(offset, normal);
}

void PlaneMath::applyMat4(glm::mat4 matrix, glm::mat3 normalMatrix) {
	glm::vec3 referencePoinst = coplanarPoint();
	referencePoinst = glm::vec3(matrix*glm::vec4(referencePoinst.x, referencePoinst.y, referencePoinst.z, 1.0));
	glm::vec3 tmpNormal = glm::normalize(normalMatrix * normal);
	constant = -glm::dot(referencePoinst, normal);
}

bool PlaneMath::equals(const PlaneMath& plane) const {
	return (normal == plane.normal && constant == plane.constant);
}

float PlaneMath::computeSurfaceArea() const {
	//TODO
	return constant;
}