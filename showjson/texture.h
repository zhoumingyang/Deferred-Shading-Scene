#pragma once
#ifndef TEXTURE_BASIC_H
#define TEXTURE_BASIC_H
#include <iostream>
#include "GL/glew.h"
#include "GL/glut.h"
#pragma comment(lib,"glew32.lib")
#define MAXTEXTURECOUNT 1000

class Texture {
private:
	GLuint* textures;
	int count;
public:
	Texture();
	Texture(const int& _count);
	virtual ~Texture();

	virtual int getTextureCount() const;
	virtual GLuint* getTextureObjects() const;
	virtual GLuint getTexObjectByIndex(const int& index = 0) const;

	virtual void bind(const int& index = 0) const;
	virtual void unBind() const;
	virtual void active(const int& slot = 0, const int& index = 0) const;
	virtual void setWrapMode(const GLenum& axis, const GLint& type) const;
	virtual void setFilterMode(const GLenum& filter, const GLint& type) const;

	virtual void setTextureData(const int& index, const void* data, const GLint& innerFormat,
		const int& width, const int& height, const GLenum& format, const GLenum& type = GL_UNSIGNED_BYTE) const;

	virtual void setOridinaryTexParam() const;
};

#endif // !TEXTURE_BASIC_H