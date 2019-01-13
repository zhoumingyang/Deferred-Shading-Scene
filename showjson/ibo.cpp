#include "ibo.h"

IBO::IBO() {
	count = 1;
	ibos = new GLuint[count];
	glGenBuffers(1, ibos);
}

IBO::IBO(const int& _count) {
	count = _count;
	if (_count <= 0) {
		count = 1;
	}
	if (count > MAXBUFFEROBJECTS) {
		count = MAXBUFFEROBJECTS;
	}
	ibos = new GLuint[count];
	glGenBuffers(count, ibos);
}

IBO::~IBO() {
	if (ibos != NULL) {
		glDeleteBuffers(count, ibos);
		count = 0;
		delete ibos;
	}
}

void IBO::setElementData(const int& index, const GLvoid* data, GLsizeiptr size) const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibos[index]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void IBO::bind(const int& index) const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibos[index]);
}

void IBO::unBind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLuint * IBO::getElementObjects() const {
	return ibos;
}

GLuint IBO::getElementObjectByIndex(const int& index) const {
	return ibos[index];
}

int IBO::getElementObjectsCount() const {
	return count;
}