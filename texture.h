#ifndef TEXTURE_H
#define TEXTURE_H

#include "perlin_noise.h"
#include "ray_tracer_stb_image.h"

class Texture {
	Color albedo;

public:
	virtual ~Texture() = default;

	virtual Color value(double u, double v, const Point3& p) const = 0;
};

class SolidColor : public Texture {
	Color albedo;

public:
	SolidColor(const Color& albedo) : albedo(albedo) {}

	SolidColor(double red, double green, double blue) : SolidColor(Color(red, green, blue)) {}

	Color value(double u, double v, const Point3& p) const override {
		return albedo;
	}
};

class TextureChecker : public Texture {
	double invScale;
	shared_ptr<Texture> even;
	shared_ptr<Texture> odd;

public:
	TextureChecker(double scale, shared_ptr<Texture> even, shared_ptr<Texture> odd) 
		: invScale(1.0 / scale), even(even), odd(odd) {}

	TextureChecker(double scale, const Color& c1, const Color& c2) 
		: TextureChecker(scale, make_shared<SolidColor>(c1), make_shared<SolidColor>(c2)) {}

	Color value(double u, double v, const Point3& p) const override {
		int x_Integer = int(std::floor(invScale * p.x()));
		int y_Integer = int(std::floor(invScale * p.y()));
		int z_Integer = int(std::floor(invScale * p.z()));

		bool isEven = (x_Integer + y_Integer + z_Integer) % 2 == 0;

		return (isEven) ? even->value(u, v, p) : odd->value(u, v, p);
	}
};

class ImageTexture : public Texture {
	RTW_image image;

public:

	ImageTexture(const char* filename) : image(filename) {}

	Color value(double u, double v, const Point3& p) const override {
		if (image.height() <= 0) return Color(0, 1, 1);

		u = Interval(0, 1).clamp(u);
		v = 1.0 - Interval(0, 1).clamp(v);

		int i = int(u * image.width());
		int k = int(v * image.height());
		const unsigned char* pixel = image.pixel_data(i, k);

		double colorScale = 1.0 / 255.0;

		return Color(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);
	}
};

class NoiseTexture : public Texture {
	PerlinNoise noise;
	double scale;

public:
	NoiseTexture(double scale) : scale(scale) {}

	Color value(double u, double v, const Point3& p) const override {
		return Color(0.5, 0.5, 0.5) * (1 + std::sin(scale * p.z() + 10 * noise.turb(p, 7)));
	}
};

#endif