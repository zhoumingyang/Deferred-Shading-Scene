#pragma once
#ifndef TEXTURE_CUBE_H
#define TEXTURE_CUBE_H
#include "texture.h"

class TextureCube : public Texture {
private:
	GLuint * textures;
	int count;
public:
	TextureCube();
	TextureCube(const int& _count);
	virtual ~TextureCube();
	virtual void bind(const int& index = 0) const;
	virtual void unBind() const;
	virtual void active(const int& slot = 0, const int& index = 0) const;
	virtual void setWrapMode(const GLenum& axis, const GLint& type) const;
	virtual void setFilterMode(const GLenum& filter, const GLint& type) const;
	virtual void setTextureData(const int& index, const void* data, const GLint& innerFormat,
		const int& width, const int& height, const GLenum& format, const GLenum& type = GL_UNSIGNED_BYTE) const;
};
#endif // !TEXTURE_CUBE_H