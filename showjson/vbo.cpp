#include "vbo.h"

VBO::VBO() {
	count = 1;
	vbos = new GLuint[count];
	glGenBuffers(count, vbos);
}

VBO::VBO(const int& _count) {
	count = _count;
	if (_count <= 0) {
		count = 1;
	}
	if (_count > MAXBUFFEROBJECTS) {
		count = MAXBUFFEROBJECTS;
	}
	vbos = new GLuint[count];
	glGenBuffers(count, vbos);
}

VBO::~VBO() {
	if (vbos != NULL) {
		glDeleteBuffers(count, vbos);
		count = 0;
		delete vbos;
	}
}

void VBO::setBufferData(const int& index, const GLvoid* data, GLsizeiptr size) const {
	glBindBuffer(GL_ARRAY_BUFFER, vbos[index]);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VBO::setAttributePoint(const int& attrLocation, const int& size, GLenum type, GLboolean normalize, GLsizei stride, const GLvoid * pointer) const {
	glVertexAttribPointer(attrLocation, size, type, normalize, stride, pointer);
	glEnableVertexAttribArray(attrLocation);
}

void VBO::bind(const int& index) const {
	glBindBuffer(GL_ARRAY_BUFFER, vbos[index]);
}

void VBO::unBind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint* VBO::getBufferObjects() const {
	return vbos;
}

int VBO::getBufferObjectsCount() const {
	return count;
}

GLuint VBO::getBufferObjectByIndex(const int& index) const {
	return vbos[index];
}