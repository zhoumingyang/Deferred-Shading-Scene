#pragma once
#ifndef VBO_H
#define VBO_H
#include <iostream>
#include "GL/glew.h"
#include "GL/glut.h"
#pragma comment(lib,"glew32.lib")
#define MAXBUFFEROBJECTS 1000

class VBO {
private:
	GLuint *vbos;
	int count;
public:
	VBO();
	VBO(const int& _count);
	~VBO();
	void setBufferData(const int& index, const GLvoid* data, GLsizeiptr size) const;
	void setAttributePoint(const int& attrLocation, const int& size, GLenum type, GLboolean normalize, GLsizei stride, const GLvoid * pointer) const;
	void bind(const int& index) const;
	void unBind() const;
	GLuint *getBufferObjects() const;
	int getBufferObjectsCount() const;
	GLuint getBufferObjectByIndex(const int& index) const;
};
#endif // !VBO_H
