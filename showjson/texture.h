#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#pragma comment(lib,"glew32.lib")
#define MAXTEXTURECOUNT 1000

class Texture {
private:
	GLuint * textures;
	int count;
public:
	Texture();
	Texture(const int& _count);
	~Texture();
	void bind(const int& index) const;
	void unBind() const;
	void active(const int& slot, const int& index) const;
	void setWrapMode(const GLenum& axis, const GLint& type) const;
	void setFilterMode(const GLenum& filter, const GLint& type) const;
	GLuint* getTextureObjects() const;
	GLuint getTexObjectByIndex(const int& index = 0) const;
	int getTextureCount() const;
	void setTextureData(const int& index, const void* data, const GLint& innerFormat, 
		const int& width, const int& height, const GLenum& format) const;
	void setOridinaryTexParam() const;
};
#endif // !TEXTURE_H