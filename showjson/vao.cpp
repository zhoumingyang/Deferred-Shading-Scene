#include "vao.h"

VAO::VAO() {
	count = 1;
	vaos = new GLuint[count];
	glGenVertexArrays(count, vaos);
}

VAO::VAO(const int& _count) {
	count = _count;
	if (_count <= 0) {
		count = 1;
	}
	if (_count > MAXARRAYTROBJECTS) {
		count = MAXARRAYTROBJECTS;
	}
	vaos = new GLuint[count];
	glGenVertexArrays(count, vaos);
}

VAO::~VAO() {
	if (vaos != NULL) {
		unBind();
		glDeleteVertexArrays(count, vaos);
		delete[] vaos;
		vaos = NULL;
		count = 0;
	}
}

void VAO::bind(const int& index) const {
	glBindVertexArray(vaos[index]);
}

void VAO::unBind() const {
	glBindVertexArray(0);
}

GLuint* VAO::getArrayObjects() const {
	return vaos;
}

GLuint VAO::getArrayObjectByIndex(const int& index) const {
	return vaos[index];
}

int VAO::getArrayObjectCount() const {
	return count;
}

void VAO::setAttribPoint(const VBO& vbo, const int& index, const int& attrLocation, const int& size, GLenum type, GLboolean normalize, GLsizei stride, const GLvoid * pointer) {
	vbo.bind(index);
	vbo.setAttributePoint(attrLocation, size, type, normalize, stride, pointer);
	vbo.unBind();
}