#pragma once
#ifndef GEO_MATH_H
#define GEO_MATH_H
#include <iostream>
#include "glm.hpp"
const float MAXF = std::numeric_limits<float>::max();
const float MINF = std::numeric_limits<float>::min();

enum GeoType {
	any,
	box3,
	sphere,
	triangle,
	plane
};

class GeoMath {
private:
	GeoType type;
public:
	GeoMath();
	GeoMath(const GeoType& _type);
	virtual ~GeoMath();
	GeoType getGeoType() const;
	void setGeoType(const GeoType& _type);
	virtual float computeSurfaceArea() const;
};

#endif // !GEO_MATH_H