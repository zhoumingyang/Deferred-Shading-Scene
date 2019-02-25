#pragma once
#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H
#include "texture.h"

class Texture2D : public Texture {
public:
	Texture2D();
	Texture2D(const int& _count);
	virtual ~Texture2D();
	virtual void bind(const int& index = 0) const;
	virtual void unBind() const;
	virtual void active(const int& slot = 0, const int& index = 0) const;
	virtual void setWrapMode(const GLenum& axis, const GLint& type) const;
	virtual void setFilterMode(const GLenum& filter, const GLint& type) const;
	virtual void setTextureData(const int& index, const void* data, const GLint& innerFormat,
		const int& width, const int& height, const GLenum& format, const GLenum& type = GL_UNSIGNED_BYTE) const;
	void setOridinaryTexParam() const;
};
#endif // !TEXTURE_2D_H