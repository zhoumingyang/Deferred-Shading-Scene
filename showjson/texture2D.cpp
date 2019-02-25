#include "texture2D.h"

Texture2D::Texture2D():Texture() {
	
}

Texture2D::Texture2D(const int& _count):Texture(_count) {
	
}

Texture2D::~Texture2D() {
	
}

void Texture2D::bind(const int& index) const {
	GLuint tex = getTexObjectByIndex(index);
	glBindTexture(GL_TEXTURE_2D, tex);
}

void Texture2D::unBind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::setWrapMode(const GLenum& axis, const GLint& type) const {
	glTexParameteri(GL_TEXTURE_2D, axis, type);
}

void Texture2D::setFilterMode(const GLenum& filter, const GLint& type) const {
	glTexParameterf(GL_TEXTURE_2D, filter, type);
}

void Texture2D::setTextureData(const int& index, const void* data, const GLint& innerFormat,
	const int& width, const int& height, const GLenum& format, const GLenum& type) const {
	glTexImage2D(GL_TEXTURE_2D, 0, innerFormat, width, height, 0, format, type, data);
}

void Texture2D::active(const int& slot, const int& index) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glEnable(GL_TEXTURE_2D);
	bind(index);
}

void Texture2D::setOridinaryTexParam() const {
	setWrapMode(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	setWrapMode(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	setFilterMode(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	setFilterMode(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}