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
		textures = NULL;
	}
	count = 0;
}

int Texture::getTextureCount() const {
	return count;
}

GLuint* Texture::getTextureObjects() const {
	return textures;
}

GLuint Texture::getTexObjectByIndex(const int& index) const {
	return textures[index];
}

void Texture::bind(const int& index) const {

}

void Texture::unBind() const {

}

void Texture::active(const int& slot, const int& index) const {

}

void Texture::setWrapMode(const GLenum& axis, const GLint& type) const {

}

void Texture::setFilterMode(const GLenum& filter, const GLint& type) const {

}

void Texture::setTextureData(const int& index, const void* data, const GLint& innerFormat,
	const int& width, const int& height, const GLenum& format, const GLenum& type) const {

}

void Texture::setOridinaryTexParam() const {

}