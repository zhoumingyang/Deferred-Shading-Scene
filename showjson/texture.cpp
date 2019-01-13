#include "texture.h"

Texture::Texture() {
	count = 1;
	textures = new GLuint[count];
	glGenTextures(count, textures);
}

Texture::Texture(const int& _count) {
	count = _count;
	if (_count <= 0) {
		count = 1;
	}
	if (_count > MAXTEXTURECOUNT) {
		count = MAXTEXTURECOUNT;
	}
	textures = new GLuint[count];
	glGenTextures(count, textures);
}

Texture::~Texture() {
	if (textures != NULL) {
		glDeleteTextures(count, textures);
		delete textures;
	}
	count = 0;
}

void Texture::bind(const int& index = 0) const {
	glBindTexture(GL_TEXTURE_2D, textures[index]);
}

void Texture::unBind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setWrapMode(const GLenum& axis, const GLint& type) const {
	glTexParameteri(GL_TEXTURE_2D, axis, type);
}

void Texture::setFilterMode(const GLenum& filter, const GLint& type) const {
	glTexParameterf(GL_TEXTURE_2D, filter, type);
}

GLuint* Texture::getTextureObjects() const {
	return textures;
}

int Texture::getTextureCount() const {
	return count;
}

GLuint Texture::getTexObjectByIndex(const int& index) const {
	return textures[index];
}

void Texture::setTextureData(const int& index, const void* data, const GLint& innerFormat,
	const int& width, const int& height, const GLenum& format) const {
	glTexImage2D(GL_TEXTURE_2D, 0, innerFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
}

void Texture::active(const int& slot = 0, const int& index = 0) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glEnable(GL_TEXTURE_2D);
	bind(index);
}

void Texture::setOridinaryTexParam() const {
	setWrapMode(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	setWrapMode(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	setFilterMode(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	setFilterMode(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}