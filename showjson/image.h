#pragma once
#ifndef IMAGE_H
#define IMAGE_H
#include <iostream>
#include <string>
//#include <GL/glew.h>
//#include <GL/glut.h>
#include "GL/glew.h"
#include "GL/glut.h"
#pragma comment(lib,"glew32.lib")
class Image {
private:
	std::string fileName;
	int components;
	int width;
	int height;
	unsigned char* data;
public:
	Image(const std::string& _fileName);
	~Image();
	bool loadImage();
	unsigned char* getImageData() const;
	GLenum getImageComponents() const;
	int getWidth() const;
	int getHeight() const;
};
#endif // !IMAGE_H