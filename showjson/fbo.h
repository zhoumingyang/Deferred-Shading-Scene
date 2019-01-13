#pragma once
#ifndef FBO_H
#define FBO_H
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include "renderbuffer.h"
#include "texture.h"
#pragma comment(lib,"glew32.lib")
#define MAXFRAMETROBJECTS 1000

class FBO {
private:
	GLuint * fbos;
	int count;
	std::vector<float> widths;
	std::vector<float> heights;
public:
	FBO();
	FBO(const std::vector<float>& _widths, const std::vector<float>& _heights, const int& _count);
	FBO(float _widths[], float _heights[], const int& _count);
	FBO(const float& _width, const float& _height);
	~FBO();
	void bind(const int& index) const;
	void unBind() const;
	GLuint* getFrameObjects() const;
	GLuint getFrameObjectByIndex(const int& index) const;
	int getFrameObjectsCount() const;
	std::vector<float> getAllWidths() const;
	std::vector<float> getAllHeights() const;
	float getFrameWidthByIndex(const int& index) const;
	float getFrameHeightByIndex(const int& index) const;
	void attachRenderBuffer(const RenderBuffer& renderBuffer) const;
	void attachTexture(const Texture& texture, const GLenum& attachPoint) const;
	void frameBufferStatusCheck() const;
};
#endif // !FBO_H