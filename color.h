#ifndef COLOR_H
#define COLOR_H

#include "vector3.h"
#include "interval.h"

using Color = Vector3;

void write_color(std::ostream& out, const Color& pixelColor) {
	double r = pixelColor.x();
	double g = pixelColor.y();
	double b = pixelColor.z();

	//Transform color to range 0..255
	static const Interval INTENSITY(0.000, 0.999);
	int rByte = int(256 * INTENSITY.clamp(r));
	int gByte = int(256 * INTENSITY.clamp(g));
	int bByte = int(256 * INTENSITY.clamp(b));

	//Print color
	std::cout << rByte << ' ' << gByte << ' ' << bByte << "\n";
}

#endif