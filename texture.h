#ifndef TEXTURE_H
#define TEXTURE_H

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

#endif