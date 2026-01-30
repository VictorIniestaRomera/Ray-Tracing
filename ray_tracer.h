#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <cstdlib>
#include <random>

//C++ std usings
using std::make_shared;
using std::shared_ptr;

//Constants
const double INFINIT = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

//Utility functions
inline double degrees_to_radians(double degrees) {
	return degrees * PI / 180.0;
}

inline double random_double() {
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;

	return distribution(generator);
}

inline double random_double(double min, double max) {
	return min + (max - min) * random_double();
}

inline int random_int(int min, int max) {
	return int(random_double(min, max + 1));
}

//Common headers
#include "color.h"
#include "vector3.h"
#include "ray.h"
#include "interval.h"

//Delete
#include "bvh.h"

#endif