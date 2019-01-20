#include "rectangle.h"

Rect::Rect(const float& _width, const float& _height) {
	width = _width;
	height = _height;
	setShapeName("sphere");
	init();
}

Rect::~Rect() {

}

void Rect::init() {
	std::vector<glm::vec3> vertices = {
		glm::vec3(-width/2.0, -height / 2.0, 0.0f),
		glm::vec3(width / 2.0, -height / 2.0, 0.0f),
		glm::vec3(width / 2.0, height / 2.0, 0.0f),
		glm::vec3(-width / 2.0, height / 2.0, 0.0f),
	};

	std::vector<glm::vec2> uvs = {
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f)
	};

	std::vector<glm::vec3> normals = {
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	};

	std::vector<int> indices = {
		0, 1, 2,
		2, 3, 0
	};

	setVertices(vertices);
	setUvs(uvs);
	setNormals(normals);
	setIndices(indices);
}

float Rect::getWidth() const {
	return width;
}

float Rect::getHeight() const {
	return height;
}

void Rect::update(const float& _width, const float& _height){
	width = _width;
	height = _height;
	clear();
	init();
}