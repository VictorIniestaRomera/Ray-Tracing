#ifndef RAY_H
#define RAY_H

#include "vector3.h"

class Ray {
	Point3 orig;
	Vector3 dir;
	double t;

public:
	//Constructors
	Ray() {}

	Ray(const Point3& origin, const Vector3& direction) : orig(origin), dir(direction), t(0) {}

	Ray(const Point3& origin, const Vector3& direction, double time) : orig(origin), dir(direction), t(time) {}

	//Get data
	const Point3& origin() const {
		return orig;
	}

	const Vector3& direction() const {
		return dir;
	}

	double time() const {
		return t;
	}

	Point3 at(double t) const {
		return orig + t * dir;
	}
};

#endif