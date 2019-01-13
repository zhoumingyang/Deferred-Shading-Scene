#include "renderbuffer.h"

RenderBuffer::RenderBuffer() {
	count = 1;
	rbo = new GLuint[count];
	attachs.resize(count);
	attachs[0] = GL_COLOR_ATTACHMENT0;
	glGenRenderbuffers(count, rbo);
}

RenderBuffer::RenderBuffer(const int& _count, GLenum _attachs[]) {
	count = _count;
	if (count <= 0) {
		count = 1;
	}
	for (int i = 0; i < count; i++) {
		attachs.push_back(_attachs[i]);
	}
	rbo = new GLuint[count];
	glGenRenderbuffers(count, rbo);
}

RenderBuffer::RenderBuffer(const int& _count, const std::vector<GLenum>& _attachs) {
	count = _attachs.size() > _count ? _count : _attachs.size();
	if (count <= 0) {
		count = 1;
		attachs.push_back(GL_COLOR_ATTACHMENT0);
	}
	else {
		for (int i = 0; i < count; i++) {
			attachs.push_back(_attachs[i]);
		}
	}
	rbo = new GLuint[count];
	glGenRenderbuffers(count, rbo);
}

RenderBuffer::RenderBuffer(const GLenum& attach) {
	count = 1;
	attachs.resize(count);
	attachs[0] = attach;        
	rbo = new GLuint[count];
	glGenRenderbuffers(count, rbo);
}

RenderBuffer::~RenderBuffer() {
	glDeleteRenderbuffers(count, rbo);
	if (rbo != NULL) {
		delete[] rbo;
		rbo = NULL;
	}
	count = 0;
}

void RenderBuffer::bind(const int& index) {
	glBindRenderbuffer(GL_RENDERBUFFER, rbo[index]);
}

void RenderBuffer::unBind() {
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderBuffer::renderBufferStorage(GLenum format, GLsizei width, GLsizei height) const {
	glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
}

GLuint* RenderBuffer::getRenderObjects() const {
	return rbo;
}

GLuint RenderBuffer::getRenderObjectByIndex(const int& index) const {
	return rbo[index];
}

GLuint RenderBuffer::getRenderObjectsCount() const {
	return count;
}

std::vector<GLenum> RenderBuffer::getAllAttachs() const {
	return attachs;
}

GLenum RenderBuffer::getAttachByIndex(const int& index) const {
	return attachs[index];
}