#pragma once
#ifndef RENDER_BUFFER_H
#define RENDER_BUFFER_H
#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GL/glut.h"
#pragma comment(lib,"glew32.lib")
#define MAXRENDEROBJECTS 3000

class RenderBuffer {
private:
	GLuint* rbo;
	int count;
	std::vector<GLenum> attachs;
public:
	RenderBuffer();
	RenderBuffer(const int& _count, GLenum _attachs[]);
	RenderBuffer(const int& _count, const std::vector<GLenum>& attachs);
	RenderBuffer(const GLenum& attach);
	~RenderBuffer();
	void bind(const int& index);
	void unBind();
	void renderBufferStorage(GLenum format, GLsizei width, GLsizei height) const;
	GLuint* getRenderObjects() const;
	GLuint getRenderObjectByIndex(const int& index = 0) const;
	GLuint getRenderObjectsCount() const;
	std::vector<GLenum> getAllAttachs() const;
	GLenum getAttachByIndex(const int& index = 0) const;
};
#endif // !RENDER_BUFFER_H
