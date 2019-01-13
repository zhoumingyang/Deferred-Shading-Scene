#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image::Image(const std::string& _fileName) {
	fileName = _fileName;
	components = 0;
	width = 0;
	height = 0;
	data = NULL;
}

Image::~Image() {
	fileName = "";
	components = 0;
	width = 0;
	height = 0;
	if (data != NULL) {
		delete data;
		data = NULL;
	}
}

bool Image::loadImage() {
	data = stbi_load(fileName.c_str(), &width, &height, &components, 0);
	if (data == NULL) {
		std::cout << "load texture error" << std::endl;
		return false;
	}
	return true;
}

unsigned char* Image::getImageData() const {
	return data;
}

GLenum Image::getImageComponents() const {
	return components == 3 ? GL_RGB : GL_RGBA;
}

int Image::getWidth() const {
	return width;
}

int Image::getHeight() const {
	return height;
}