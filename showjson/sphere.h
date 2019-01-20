#pragma once
#ifndef SPHERE_H
#define SPHERE_H
#include "shapegeometry.h"

// reference:
// http://www.songho.ca/opengl/gl_sphere.html

class Sphere :public ShapeGeometry {
private:
	float radius;
	int sectorCount;
	int stackCount;
	bool smooth;
	std::vector<unsigned int> lineIndices;
	std::vector<float> interleavedVertices;
	int interleavedStride;
	void updateRadius();
	std::vector<float> computeFaceNormal(float x1, float y1, float z1,
		float x2, float y2, float z2,
		float x3, float y3, float z3);
	void buildVerticesSmooth();
	void buildVerticesFlat();
	void buildInterleavedVertices();
public:
	Sphere(float _radius = 1.0f, int _sectors = 36, int _stacks = 18, bool _smooth = true);
	~Sphere();
	void init(float _radius, int _sectors, int _stacks, bool _smooth);
	void setRadius(const float& _radius);
	void setSectorCount(const int& _sectors);
	void setStackCount(const int& _stacks);
	void setSmooth(const bool& _smooth);
	float getRadius() const;
	int getSectorCount() const;
	int getStackCount() const;
	bool isSmooth() const;
	virtual void draw() {

	}
};
#endif // !SPHERE_H