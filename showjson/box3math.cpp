#include "box3math.h"
#include "spheremath.h"
#include "planemath.h"
#include "trianglemath.h"
#include "mesh.h"

Box3Math::Box3Math() {
	min = glm::vec3(MAXF, MAXF, MAXF);
	max = glm::vec3(MINF, MINF, MINF);
	type = box3;
}

Box3Math::Box3Math(const glm::vec3& _min, const glm::vec3& _max) {
	min = _min;
	max = _max;
}

Box3Math::~Box3Math() {

}

glm::vec3 Box3Math::getMin() const {
	return min;
}

glm::vec3 Box3Math::getMax() const {
	return max;
}

void Box3Math::setMin(const glm::vec3& _min) {
	min = _min;
}

void Box3Math::setMax(const glm::vec3& _max) {
	max = _max;
}

void Box3Math::set(const glm::vec3& _min, const glm::vec3& _max) {
	min = _min;
	max = _max;
}

void Box3Math::setFromMesh(const Mesh& mesh) {
	std::vector<MeshUnit> meshUnits = mesh.getMeshUnits();
	for (int i = 0; i < meshUnits.size(); i++) {
		setFromPosition(meshUnits[i].vertices);
	}
}

void Box3Math::setFromPosition(const std::vector<float>& position) {
	float minx = std::numeric_limits<float>::max();
	float miny = std::numeric_limits<float>::max();
	float minz = std::numeric_limits<float>::max();

	float maxx = -std::numeric_limits<float>::max();
	float maxy = -std::numeric_limits<float>::max();
	float maxz = -std::numeric_limits<float>::max();

	for (int i = 0; i + 2 < position.size(); i += 3) {
		minx = position[i] < minx ? position[i] : minx;
		maxx = position[i] > maxx ? position[i] : maxx;

		miny = position[i + 1] < miny ? position[i + 1] : miny;
		maxy = position[i + 1] > maxy ? position[i + 1] : maxy;

		minz = position[i + 2] < minz ? position[i + 2] : minz;
		maxz = position[i + 2] > maxz ? position[i + 2] : maxz;
	}
	min = glm::vec3(minx, miny, minz);
	max = glm::vec3(maxx, maxy, maxz);
}

void Box3Math::setFromPosition(const std::vector<glm::vec3>& position) {
	min.x = std::numeric_limits<float>::max();
	min.y = std::numeric_limits<float>::max();
	min.z = std::numeric_limits<float>::max();

	max.x = -std::numeric_limits<float>::max();
	max.y = -std::numeric_limits<float>::max();
	max.z = -std::numeric_limits<float>::max();

	for (int i = 0; i < position.size(); i++) {
		min.x = position[i].x < min.x ? position[i].x : min.x;
		min.y = position[i].y < min.y ? position[i].y : min.y;
		min.z = position[i].z < min.z ? position[i].z : min.z;
		max.x = position[i].x > max.x ? position[i].x : max.x;
		max.y = position[i].y > max.y ? position[i].y : max.y;
		max.z = position[i].z > max.z ? position[i].z : max.z;
	}
}

void Box3Math::setFromCenterAndSize(const glm::vec3& center, const glm::vec3& size) {
	glm::vec3 v1 = size;
	glm::vec3 halfSize = glm::vec3(v1.x * 0.5, v1.y * 0.5, v1.z * 0.5);
	min = center - halfSize;
	max = center + halfSize;
}

void Box3Math::makeEmpty() {
	min = glm::vec3(MAXF, MAXF, MAXF);
	max = glm::vec3(MINF, MINF, MINF);
}

bool Box3Math::isEmpty() const {
	return (max.x < min.x) || (max.y < min.y) || (max.z < min.z);
}

glm::vec3 Box3Math::getCenter() const {
	glm::vec3 result(0.0, 0.0, 0.0);
	if (!isEmpty()) {
		result = min + max;
		result.x = result.x / 2.0;
		result.y = result.y / 2.0;
		result.z = result.z / 2.0;
	}
	return result;
}

glm::vec3 Box3Math::getSize() const {
	glm::vec3 result(0.0, 0.0, 0.0);
	if (!isEmpty()) {
		result.x = max.x - min.x;
		result.y = max.y - min.y;
		result.z = max.z - min.z;
	}
	return result;
}

void Box3Math::expandByPoint(const glm::vec3& point) {
	glm::min(min, point);
	glm::max(max, point);
}

void Box3Math::expandByVector(const glm::vec3& _vetor) {
	min += _vetor;
	max += _vetor;
}

void Box3Math::expandByScalar(const float& scalar) {
	min += glm::vec3(-scalar);
	max += glm::vec3(scalar);
}

bool Box3Math::containsPoint(const glm::vec3& point) const {
	return point.x < min.x || point.x > max.x ||
		point.y < min.y || point.y > max.y ||
		point.z < min.z || point.z > max.z ? false : true;
}

bool Box3Math::containsBox(const Box3Math& box) const {
	return min.x <= box.min.x && box.max.x <= max.x &&
		min.y <= box.min.y && box.max.y <= max.y &&
		min.z <= box.min.z && box.max.z <= max.z;
}

bool Box3Math::intersectsBox(const Box3Math& box) const {
	return box.max.x < min.x || box.min.x > max.x ||
		box.max.y < min.y || box.min.y > max.y ||
		box.max.z < min.z || box.min.z > max.z ? false : true;
}

bool Box3Math::intersectsSphere(const SphereMath& sphere) const {
	float radius = sphere.getRadius();
	glm::vec3 center = sphere.getCenter();
	glm::vec3 clm = clampPoint(center);
	float distance = glm::distance(glm::vec3(), center);
	return distance < radius;
}

bool Box3Math::intersectsPlane(const PlaneMath& plane) const {
	float _min, _max;
	float constant = plane.getConstant();
	glm::vec3 normal = plane.getNormal();
	if (normal.x > 0) {
		_min = normal.x * min.x;
		_max = normal.x * max.x;
	}
	else {
		_min = normal.x * max.x;
		_max = normal.x * min.x;
	}

	if (normal.y > 0) {
		_min += normal.y * min.y;
		_max += normal.y * max.y;
	}
	else {
		_min += normal.y * max.y;
		_max += normal.y * min.y;
	}

	if (normal.z > 0) {
		_min += normal.z * min.z;
		_max += normal.z * max.z;
	}
	else {
		_min += normal.z * max.z;
		_max += normal.z * min.z;
	}
	return (_min <= -constant && _max >= -constant);
}

bool Box3Math::satForAxes(const std::vector<float>& axes, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& extents) const {
	glm::vec3 testAxis;
	for (int i = 0;i + 2 < axes.size(); i += 3) {
		testAxis = glm::vec3(axes[i], axes[i + 1], axes[i + 2]);
		float r = extents.x * abs(testAxis.x) + extents.y * abs(testAxis.y) + extents.z * abs(testAxis.z);
		float p0 = glm::dot(v0, testAxis);
		float p1 = glm::dot(v1, testAxis);
		float p2 = glm::dot(v2, testAxis);
		float tmpMax = std::max(p0, p1);
		tmpMax = std::max(tmpMax, p2);
		float tmpMin = std::min(p0, p1);
		tmpMin = std::min(tmpMin, p1);
		if (std::max(-tmpMax, tmpMin) > r) {
			return false;
		}
	}
	return true;
}

bool Box3Math::intersectsTriangle(const TriangleMath& triangle) const {
	if (isEmpty()) {
		return false;
	}
	glm::vec3 boxCenter = getCenter();
	glm::vec3 extents = max - boxCenter;
	glm::vec3 _a = triangle._a();
	glm::vec3 _b = triangle._b();
	glm::vec3 _c = triangle._c();
	glm::vec3 v0 = _a - boxCenter;
	glm::vec3 v1 = _b - boxCenter;
	glm::vec3 v2 = _c - boxCenter;
	glm::vec3 f0 = v1 - v0;
	glm::vec3 f1 = v2 - v1;
	glm::vec3 f2 = v0 - v2;

	std::vector<float> axes = {
		0, -f0.z, f0.y, 0, -f1.z, f1.y, 0, -f2.z, f2.y,
		f0.z, 0, -f0.x, f1.z, 0, -f1.x, f2.z, 0, -f2.x,
		-f0.y, f0.x, 0, -f1.y, f1.x, 0, -f2.y, f2.x, 0
	};

	if (!satForAxes(axes, v0, v1, v2, extents)) {
		return false;
	}

	axes = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
	if (!satForAxes(axes, v0, v1, v2, extents)) {
		return false;
	}

	glm::vec3 triangleNormal = glm::cross(f0, f1);
	axes = { triangleNormal.x, triangleNormal.y, triangleNormal.z };
	return satForAxes(axes, v0, v1, v2, extents);
}

glm::vec3 Box3Math::clampPoint(const glm::vec3& point) const {
	glm::vec3 result = point;
	return glm::clamp(result, min, max);
}

float Box3Math::distanceToPoint(const glm::vec3& point) const {
	glm::vec3 v1 = point;
	glm::vec3 clampedPoint = glm::clamp(v1, min, max);
	return glm::length(clampedPoint - point);
}

SphereMath Box3Math::getBoundingSphere() const {
	SphereMath sphere;
	glm::vec3 center = getCenter();
	glm::vec3 size = getSize();
	float radius = glm::length(size);
	sphere.setCenter(center);
	sphere.setRadius(radius);
	return sphere;
}

Box3Math Box3Math::intersect(const Box3Math& box) {
	glm::vec3 tmin = glm::max(min, box.getMin());
	glm::vec3 tmax = glm::min(max, box.getMax());
	return Box3Math(tmin, tmax);
}

Box3Math Box3Math::unionBox(const Box3Math& box) {
	glm::vec3 tmin = glm::max(min, box.getMin());
	glm::vec3 tmax = glm::min(max, box.getMax());
	return Box3Math(tmin, tmax);
}

void Box3Math::applyMat4(glm::mat4 matrix) {
	std::vector<glm::vec3> points(8);
	points[0] = glm::vec3(matrix * glm::vec4(min.x, min.y, min.z, 1.0));
	points[1] = glm::vec3(matrix * glm::vec4(min.x, min.y, max.z, 1.0));
	points[2] = glm::vec3(matrix * glm::vec4(min.x, max.y, min.z, 1.0));
	points[3] = glm::vec3(matrix * glm::vec4(min.x, max.y, max.z, 1.0));
	points[4] = glm::vec3(matrix * glm::vec4(max.x, min.y, min.z, 1.0));
	points[5] = glm::vec3(matrix * glm::vec4(max.x, min.y, max.z, 1.0));
	points[6] = glm::vec3(matrix * glm::vec4(max.x, max.y, min.z, 1.0));
	points[7] = glm::vec3(matrix * glm::vec4(max.x, max.y, max.z, 1.0));
	setFromPosition(points);
}

void Box3Math::translate(glm::vec3 offset) {
	min += offset;
	max += offset;
}

bool Box3Math::equals(const Box3Math& box) const {
	return min == box.min && max == box.max;
}

float Box3Math::computeSurfaceArea() const {
	//TODO
	return 10;
}