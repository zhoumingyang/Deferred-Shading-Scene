#include "spheremath.h"
#include "box3math.h"
#include "planemath.h"

SphereMath::SphereMath() {
	radius = 0.0;
	center = glm::vec3(0.0, 0.0, 0.0);
	type = sphere;
}

SphereMath::SphereMath(const glm::vec3& _center, const float& _radius) {
	center = _center;
	radius = _radius;
}

SphereMath::~SphereMath() {

}

float SphereMath::getRadius() const {
	return radius;
}

glm::vec3 SphereMath::getCenter() const {
	return center;
}

void SphereMath::setRadius(const float& _radius) {
	radius = _radius;
}


void SphereMath::setCenter(const glm::vec3& _center) {
	center = _center;
}

void SphereMath::set(const glm::vec3& _center, const float& _radius) {
	center = _center;
	radius = _radius;
}

void SphereMath::setFromPosition(const std::vector<float>& position) {
	Box3Math box;
	box.setFromPosition(position);
	center = box.getCenter();
	float maxRadiusSq = 0.0;
	for (int i = 0; i + 2 < position.size(); i += 3) {
		glm::vec3 tmpPoint = glm::vec3(position[i], position[i + 1], position[i + 2]);
		float tmpL = glm::distance(center, tmpPoint);
		maxRadiusSq = maxRadiusSq < tmpL ? tmpL : maxRadiusSq;
	}
	radius = maxRadiusSq;
}

void SphereMath::setFromPosition(const std::vector<glm::vec3>& position) {
	Box3Math box;
	box.setFromPosition(position);
	center = box.getCenter();
	float maxRadiusSq = 0.0;
	for (int i = 0; i < position.size(); i++) {
		float tmpL = glm::distance(center, position[i]);
		maxRadiusSq = maxRadiusSq < tmpL ? tmpL : maxRadiusSq;
	}
	radius = maxRadiusSq;
}

bool SphereMath::empty() const {
	return radius <= 0;
}

bool SphereMath::containsPoint(const glm::vec3& point) const {
	float tmpL = glm::distance(center, point);
	return tmpL <= radius;
}

float SphereMath::distanceToPoint(const glm::vec3& point) const {
	return glm::distance(point, center) - radius;
}

bool SphereMath::intersectsSphere(const SphereMath& sphere) const {
	float radiusSum = radius + sphere.radius;
	return glm::distance(sphere.center, sphere.center) <= radiusSum;
}

bool SphereMath::intersectsBox(const Box3Math& box) const {
	return box.intersectsSphere(*this);
}

bool SphereMath::intersectsPlane(const PlaneMath& plane) const {
	return abs(plane.distanceToPoint(center)) <= radius;
}

glm::vec3 SphereMath::clampPoint(const glm::vec3& point) const {
	glm::vec3 result = point;
	float deltaLength = glm::distance(center, point);
	if (deltaLength > radius) {
		result = glm::normalize(result - center);
		result = result * radius + center;
	}
	return result;
}

Box3Math SphereMath::getBoundingBox() const {
	Box3Math result;
	result.set(center, center);
	result.expandByScalar(radius);
	return result;
}

void SphereMath::applyMat4(glm::mat4 matrix) {
	float scaleXSq = matrix[0][0] * matrix[0][0] + matrix[1][0] * matrix[1][0] + matrix[2][0] * matrix[2][0];
	float scaleYSq = matrix[0][1] * matrix[0][1] + matrix[1][1] * matrix[1][1] + matrix[2][1] * matrix[2][1];
	float scaleZSq = matrix[0][2] * matrix[0][2] + matrix[1][2] * matrix[1][2] + matrix[2][2] * matrix[2][2];
	float maxScale = scaleXSq > scaleYSq ? scaleXSq : scaleYSq;
	maxScale = maxScale < scaleZSq ? scaleZSq : maxScale;
	maxScale = sqrt(maxScale);
	center = glm::vec3(matrix * glm::vec4(center, 1.0));
	radius = radius * maxScale;
}

void SphereMath::translate(glm::vec3 offset) {
	center = center + offset;
}

bool SphereMath::equals(const SphereMath& sphere) const {
	return center == sphere.getCenter() && radius == sphere.getRadius();
}

float SphereMath::computeSurfaceArea() const {
	return radius;
}