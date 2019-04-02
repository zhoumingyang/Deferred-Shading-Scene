#include "geomath.h"

GeoMath::GeoMath() {
	type = any;
}

GeoMath::GeoMath(const GeoType& _type) {
	type = _type;
}

GeoMath::~GeoMath() {

}

GeoType GeoMath::getGeoType() const {
	return type;
}

void GeoMath::setGeoType(const GeoType& _type) {
	type = _type;
}

float GeoMath::computeSurfaceArea() const {
	return 0.0;
}