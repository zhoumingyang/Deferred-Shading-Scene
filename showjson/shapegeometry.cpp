#include "shapegeometry.h"

ShapeGeometry::ShapeGeometry() {
	position = glm::vec3(0.0, 0.0, 0.0);
	scale = glm::vec3(1.0, 1.0, 1.0);
	rot = glm::vec4(0.0, 0.0, 0.0, 1.0);
}

ShapeGeometry::~ShapeGeometry() {

}

void ShapeGeometry::clear() {
	std::vector<glm::vec3>().swap(pos);
	std::vector<glm::vec2>().swap(uvs);
	std::vector<glm::vec3>().swap(normals);
	std::vector<int>().swap(indices);
}

std::string ShapeGeometry::getShapeName() const {
	return shapeName;
}

void ShapeGeometry::setShapeName(const std::string& _shapeName) {
	shapeName = _shapeName;
}

std::vector<glm::vec3> ShapeGeometry::getVertices() const {
	return pos;
}

void ShapeGeometry::setVertices(const std::vector<glm::vec3>& _pos) {
	pos = _pos;
}

std::vector<glm::vec2> ShapeGeometry::getUvs() const {
	return uvs;
}

void ShapeGeometry::setUvs(const std::vector<glm::vec2>& _uvs) {
	uvs = _uvs;
}

std::vector<glm::vec3> ShapeGeometry::getNormals() const {
	return normals;
}

void ShapeGeometry::setNormals(const std::vector<glm::vec3>& _normals) {
	normals = _normals;
}

std::vector<int> ShapeGeometry::getIndices() const {
	return indices;
}

void ShapeGeometry::setIndices(const std::vector<int>& _indices) {
	indices = _indices;
}

void ShapeGeometry::pushVertex(const float& x, const float& y, const float& z) {
	pos.push_back(glm::vec3(x, y, z));
}

void ShapeGeometry::pushVertex(const glm::vec3& ver) {
	pos.push_back(ver);
}

void ShapeGeometry::pushUv(const float& u, const float& v) {
	uvs.push_back(glm::vec2(u, v));
}

void ShapeGeometry::pushUv(const glm::vec2& uv) {
	uvs.push_back(uv);
}

void ShapeGeometry::pushNormal(const float& x, const float& y, const float& z) {
	normals.push_back(glm::vec3(x, y, z));
}


void ShapeGeometry::pushNormal(const glm::vec3& normal) {
	normals.push_back(normal);
}

void ShapeGeometry::addIndex(unsigned int i1, unsigned int i2, unsigned int i3) {
	indices.push_back(i1);
	indices.push_back(i2);
	indices.push_back(i3);
}

int ShapeGeometry::getVerticesCount() const {
	return pos.size();
}

int ShapeGeometry::getUvsCount() const {
	return uvs.size();
}

int ShapeGeometry::getNormalsCount() const {
	return normals.size();
}

int ShapeGeometry::getIndicesCount() const {
	return indices.size();
}

void ShapeGeometry::setColor(const float& x, const float& y, const float& z) {
	color = glm::vec3(x, y, z);
}

void ShapeGeometry::setColor(const glm::vec3& _color) {
	color = _color;
}

void ShapeGeometry::setPosition(const float& x, const float& y, const float& z) {
	position = glm::vec3(x, y, z);
}

void ShapeGeometry::setPosition(const glm::vec3& _position) {
	position = _position;
}

void ShapeGeometry::setScale(const float& x, const float& y, const float& z) {
	scale = glm::vec3(x, y, z);
}

void ShapeGeometry::setScale(const glm::vec3& _scale) {
	scale = _scale;
}

void ShapeGeometry::setRot(const float& x, const float& y, const float& z, const float& w) {
	rot = glm::vec4(x, y, z, w);
}

void ShapeGeometry::setRot(const glm::vec4& _rot) {
	rot = _rot;
}

glm::vec3 ShapeGeometry::getPosition() const {
	return position;
}

glm::vec3 ShapeGeometry::getScale() const {
	return scale;
}

glm::vec4 ShapeGeometry::getRot() const {
	return rot;
}

glm::vec3 ShapeGeometry::getColor() const {
	return color;
}

glm::mat4 ShapeGeometry::getModelMatrix() const {
	glm::mat4 posMat4 = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 scaleMat4 = glm::scale(glm::mat4(1.0f), scale);
	glm::mat4 rotMat4 = glm::mat4_cast(glm::quat(rot.w, rot.x, rot.y, rot.z));
	return posMat4 * rotMat4 * scaleMat4;
}

glm::mat4 ShapeGeometry::getNormalMatrix() const {
	glm::mat4 scaleMat4 = glm::scale(glm::mat4(1.0f), scale);
	glm::mat4 rotMat4 = glm::mat4_cast(glm::quat(rot.w, rot.x, rot.y, rot.z));
	return rotMat4 * scaleMat4;
}