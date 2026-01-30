#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class Material {
public:
	virtual ~Material() = default;

	virtual bool scatter(const Ray& r, const HitRecord& rec, Color& attenuation, Ray& scattered) const {
		return false;
	}
};

class Lambertian : public Material {
	Color albedo;

public:
	Lambertian(const Color& albedo) : albedo(albedo) {}

	bool scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
		Vector3 scatterDirection = rec.normal + random_unit_vector();

		//Catch degenerated scatter direction
		if (scatterDirection.near_zero()) scatterDirection = rec.normal;

		scattered = Ray(rec.p, scatterDirection, rIn.time());
		attenuation = albedo;

		return true;
	}
};

class Metal : public Material {
	Color albedo;
	double fuzz;

public:
	Metal (const Color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

	bool scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
		Vector3 reflected = reflect(rIn.direction(), rec.normal);
		reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
		scattered = Ray(rec.p, reflected, rIn.time());
		attenuation = albedo;

		return (dot(scattered.direction(), rec.normal) > 0);
	}
};

class Dielectric : public Material {
	double refractionIndex;

	static double reflectance(double cosine, double reflactionIndex) {
		//Use Schlick's approximation for reflectance
		double r0 = (1 - reflactionIndex) / (1 + reflactionIndex);
		r0 *= r0;

		return r0 + (1 - r0) * std::pow((1 - cosine), 5);
	}

public:
	Dielectric(double refractionIndex) : refractionIndex(refractionIndex) {}

	bool scatter(const Ray& rIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
		attenuation = Color(1.0, 1.0, 1.0);
		double ri = rec.frontFace ? (1.0 / refractionIndex) : refractionIndex;

		Vector3 unitDirection = unit_vector(rIn.direction());
		double cosTheta = std::fmin(dot(-unitDirection, rec.normal), 1.0);
		double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

		bool cannotRefract = ri * sinTheta > 1.0;
		Vector3 direction;

		if (cannotRefract || reflectance(cosTheta, ri) > random_double()) direction = reflect(unitDirection, rec.normal);
		else direction = refract(unitDirection, rec.normal, ri);

		scattered = Ray(rec.p, direction, rIn.time());

		return true;
	}
};

#endif