#include "texturecube.h"

TextureCube::TextureCube():Texture(){

}

TextureCube::TextureCube(const int& _count):Texture(_count) {
	
}

TextureCube::~TextureCube() {

}

void TextureCube::bind(const int& index) const {
	GLuint tex = getTexObjectByIndex(index);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
}

void TextureCube::unBind() const {
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void TextureCube::active(const int& slot, const int& index) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glEnable(GL_TEXTURE_CUBE_MAP);
	bind(index);
}

void TextureCube::setWrapMode(const GLenum& axis, const GLint& type) const {
	glTexParameteri(GL_TEXTURE_CUBE_MAP, axis, type);
}

void TextureCube::setFilterMode(const GLenum& filter, const GLint& type) const {
	glTexParameterf(GL_TEXTURE_CUBE_MAP, filter, type);
}

void TextureCube::setTextureData(const int& index, const void* data, const GLint& innerFormat,
	const int& width, const int& height, const GLenum& format, const GLenum& type) const {
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, innerFormat, width, height, 0, format, type, data);
}