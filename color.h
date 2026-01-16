#ifndef COLOR_H
#define COLOR_H

#include <iostream>

#include "vector3.h"

using Color = Vector3;

void write_color(std::ostream& out, const Color& pixelColor) {
	double r = pixelColor.x();
	double g = pixelColor.y();
	double b = pixelColor.z();

	//Transform color to range 0..255
	int rByte = int(256 * r);
	int gByte = int(256 * g);
	int bByte = int(256 * b);

	//Print color
	std::cout << rByte << ' ' << gByte << ' ' << bByte << "\n";
}

#endif