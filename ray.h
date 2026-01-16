#ifndef RAY_H
#define RAY_H

#include "vector3.h"

class Ray {
	Point3 orig;
	Vector3 dir;

public:
	//Constructors
	Ray() {}

	Ray(const Point3& origin, const Vector3& direction) : orig(origin), dir(direction) {}

	//Get data
	const Point3& origin() const {
		return orig;
	}

	const Vector3& direction() const {
		return dir;
	}
};

#endif