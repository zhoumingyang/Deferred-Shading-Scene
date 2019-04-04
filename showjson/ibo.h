#pragma once
#ifndef IBO_H
#define IBO_H
#include <iostream>
#include "GL/glew.h"
#include "GL/glut.h"
#pragma comment(lib,"glew32.lib")
#define MAXBUFFEROBJECTS 1000

class IBO {
private:
	GLuint *ibos;
	int count;
public:
	IBO();
	IBO(const int& _count);
	~IBO();
	void setElementData(const int& index,  const GLvoid* data, GLsizeiptr size) const;
	void bind(const int& index) const;
	void unBind() const;
	GLuint * getElementObjects() const;
	int getElementObjectsCount() const;
	GLuint getElementObjectByIndex(const int& index) const;
};
#endif // !IBO_H
