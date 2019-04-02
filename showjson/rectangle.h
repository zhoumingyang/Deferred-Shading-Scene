#pragma once
#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "shapegeometry.h"

class Rect:public ShapeGeometry {
private:
	float width;
	float height;
	void init();
public:
	Rect(const float& _width = 1.88f, const float& _height = 1.88f);
	~Rect();
	float getWidth() const;
	float getHeight() const;
	void update(const float& _width, const float& _height);
	virtual void draw() {

	}
};
#endif // !RECTANGLE_H