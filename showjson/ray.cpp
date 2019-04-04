#include "ray.h"
#include "box3math.h"
#include "spheremath.h"
#include "planemath.h"
#include "trianglemath.h"

Ray::Ray() {
	origin = glm::vec3(0.0, 0.0, 0.0);
	direction = glm::vec3(0.0, 0.0, -1.0);
}

Ray::Ray(const glm::vec3& _origin, const glm::vec3& _direction) {
	origin = _origin;
	direction = _direction;
}

Ray::~Ray() {

}

void Ray::set(const glm::vec3& _origin, const glm::vec3& _direction) {
	origin = _origin;
	direction = _direction;
}

void Ray::setOrigin(const glm::vec3& _origin) {
	origin = _origin;
}

void Ray::setDirection(const glm::vec3& _direction) {
	direction = _direction;
}

glm::vec3 Ray::getOrigin() const {
	return origin;
}

glm::vec3 Ray::getDirection() const {
	return direction;
}

glm::vec3 Ray::at(const float& t) const {
	glm::vec3 tmpDireciton = direction;
	tmpDireciton *= t;
	glm::vec3 atPoint = origin + tmpDireciton;
	return atPoint;
}

glm::vec3 Ray::lookat(const glm::vec3& v) {
	direction = v;
	glm::vec3 result = direction - origin;
	return result;
}

Ray Ray::recast(const float& t) {
	origin = at(t);
	return *this;
}

glm::vec3 Ray::closestPointToPoint(const glm::vec3& point) const {
	glm::vec3 target = point - origin;
	float directionDistance = glm::dot(target, direction);
	if (directionDistance < 0) {
		return origin;
	}
	target = direction;
	return target * directionDistance + origin;
}

float Ray::distanceToPoint(const glm::vec3& point) const {
	return sqrt(distanceSqToPoint(point));
}

float Ray::distanceSqToPoint(const glm::vec3& point) const {
	float directionDistance = glm::dot(point - origin, direction);
	if (directionDistance < 0) {
		return glm::distance(origin, point);
	}
	glm::vec3 v1 = direction;
	v1 = v1 * directionDistance + origin;
	return glm::distance(v1, point);
}

glm::vec3 Ray::intersectBox(const Box3Math& box3) const {
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	float invdirx = 1 / direction.x;
	float invdiry = 1 / direction.y;
	float invdirz = 1 / direction.z;
	glm::vec3 tmpOrigin = origin;
	glm::vec3 boxMin = box3.getMin();
	glm::vec3 boxMax = box3.getMax();
	if (invdirx >= 0) {
		tmin = (boxMin.x - tmpOrigin.x) * invdirx;
		tmax = (boxMax.x - tmpOrigin.x) * invdirx;
	}
	else {
		tmin = (boxMax.x - tmpOrigin.x) * invdirx;
		tmax = (boxMin.x - tmpOrigin.x) * invdirx;
	}

	if (invdiry >= 0) {
		tymin = (boxMin.y - tmpOrigin.y) * invdiry;
		tymax = (boxMax.y - tmpOrigin.y) * invdiry;
	}
	else {
		tymin = (boxMax.y - tmpOrigin.y) * invdiry;
		tymax = (boxMin.y - tmpOrigin.y) * invdiry;
	}

	if ((tmin > tymax) || (tymin > tmax)) { 
		return glm::vec3(NULL); 
	}

	if (tymin > tmin || tmin != tmin) tmin = tymin;
	if (tymax < tmax || tmax != tmax) tmax = tymax;

	if (invdirz >= 0) {
		tzmin = (boxMin.z - tmpOrigin.z) * invdirz;
		tzmax = (boxMax.z - tmpOrigin.z) * invdirz;
	}
	else {
		tzmin = (boxMax.z - tmpOrigin.z) * invdirz;
		tzmax = (boxMin.z - tmpOrigin.z) * invdirz;
	}

	if ((tmin > tzmax) || (tzmin > tmax)) { 
		return glm::vec3(NULL); 
	}

	if (tzmin > tmin || tmin != tmin) tmin = tzmin;
	if (tzmax < tmax || tmax != tmax) tmax = tzmax;

	if (tmax < 0) { 
		return glm::vec3(NULL); 
	}
	return at(tmin >= 0 ? tmin : tmax);
}

bool Ray::intersectsBox(const Box3Math& box3) const {
	glm::vec3 v = intersectBox(box3);
	if (v.x == NULL || v.y == NULL || v.z == NULL) {
		return false;
	}
	return true;
}

glm::vec3 Ray::intersectSphere(const SphereMath& sphere) const {
	glm::vec3 sphereCenter = sphere.getCenter();
	float sphereRadius = sphere.getRadius();
	glm::vec3 v1 = sphereCenter - origin;
	float tca = glm::dot(v1, direction);
	float d2 = glm::dot(v1, v1) - tca * tca;
	float radius2 = sphereRadius * sphereRadius;
	float thc = sqrt(radius2 - d2);
	float t0 = tca - thc;
	float t1 = tca + thc;
	if (t0 < 0 && t1 < 0) return glm::vec3(NULL);
	if (t0 < 0) return at(t1);
	return at(t0);
}

bool Ray::intersectsSphere(const SphereMath& sphere) const {
	float radius = sphere.getRadius();
	return distanceSqToPoint(sphere.getCenter()) <= radius * radius;
}

float Ray::distanceToPlane(const PlaneMath& plane) const {
	glm::vec3 normal = plane.getNormal();
	float constant = plane.getConstant();
	float denominator = glm::dot(normal, direction);
	if (denominator == 0) {
		if (plane.distanceToPoint(origin) == 0) {
			return 0;
		}
		return NULL;
	}
	float t = -(glm::dot(origin, normal) + constant) / denominator;
	return t >= 0 ? t : NULL;
}

glm::vec3 Ray::intersectPlane(const PlaneMath& plane) const {
	float t = distanceToPlane(plane);
	if (t == NULL) {
		return glm::vec3(NULL);
	}
	return at(t);
}

bool Ray::intersectsPlane(const PlaneMath& plane) const {
	float distToPoint = plane.distanceToPoint(origin);
	if (distToPoint == 0) {
		return true;
	}

	float denominator = glm::dot(plane.getNormal(), direction);
	if (denominator * distToPoint) {
		return true;
	}

	return false;
}

glm::vec3 Ray::intersectTriangle(const glm::vec3& a, const glm::vec3& b, 
	const glm::vec3&c, const bool& backfaceCulling) const {
	glm::vec3 diff;
	glm::vec3 edge1 = b - a;
	glm::vec3 edge2 = c - a;
	glm::vec3 normal = glm::cross(edge1, edge2);

	float DdN = glm::dot(direction, normal);
	int sign = 1;
	if (DdN > 0) {
		if (backfaceCulling) return glm::vec3(NULL);
		sign = 1;
	}
	else if (DdN < 0) {
		sign = -1;
		DdN = -DdN;
	}
	else {
		return glm::vec3(NULL);
	}

	diff = origin - a;
	float DdQxE2 = sign * glm::dot(direction, glm::cross(diff, edge2));
	if (DdQxE2 < 0) {
		return glm::vec3(NULL);
	}

	float DdE1xQ = sign * glm::dot(direction, glm::cross(edge1, diff));
	if (DdE1xQ < 0) {
		return glm::vec3(NULL);
	}

	if (DdQxE2 + DdE1xQ > DdN) {
		return glm::vec3(NULL);
	}

	float QdN = -sign * glm::dot(diff, normal);

	if (QdN < 0) {
		return glm::vec3(NULL);
	}

	return at(QdN / DdN);
}

bool Ray::equals(const Ray& ray) const {
	return ray.origin == origin && ray.direction == direction;
}

void Ray::applyMat4(const glm::mat4& mat) {
	origin = glm::vec3(mat * glm::vec4(origin, 1.0));
	glm::vec3 tmp = direction;
	direction.x = mat[0][0] * tmp.x + mat[0][1] * tmp.y + mat[0][2] * tmp.z;
	direction.y = mat[1][0] * tmp.x + mat[1][1] * tmp.y + mat[1][2] * tmp.z;
	direction.z = mat[2][0] * tmp.x + mat[2][1] * tmp.y + mat[2][2] * tmp.z;
}