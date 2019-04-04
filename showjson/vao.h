#pragma once
#ifndef VAO_H
#define VAO_H
#include <iostream>
#include "GL/glew.h"
#include "GL/glut.h"
#include "vbo.h"
#pragma comment(lib,"glew32.lib")
#define MAXARRAYTROBJECTS 1000

class VAO {
private:
	GLuint * vaos;
	int count;
public:
	VAO();
	VAO(const int& _count);
	~VAO();
	void bind(const int& index) const;
	void unBind() const;
	void setAttribPoint(const VBO& vbo, const int& index, const int& attrLocation, const int& size, GLenum type, GLboolean normalize, GLsizei stride, const GLvoid * pointer);
	GLuint* getArrayObjects() const;
	GLuint getArrayObjectByIndex(const int& index) const;
	int getArrayObjectCount() const;
};
#endif // !VAO_H