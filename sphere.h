#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class Sphere : public Hittable {
	Ray center;
	double radius;
	shared_ptr<Material> mat;
	AABB bbox;

	static void get_sphere_uv(const Point3& p, double& u, double& v) {
		double theta = std::acos(-p.y());
		double phi = std::atan2(-p.z(), p.x()) + PI;

		u = phi / (2 * PI);
		v = theta / PI;
	}

public:
	//Static sphere
	Sphere (const Point3& staticCenter, double radius, shared_ptr<Material> mat)
		: center(staticCenter, Vector3(0, 0, 0)), radius(std::fmax(0, radius)), mat(mat) {
		Vector3 rvec = Vector3(radius, radius, radius);
		bbox = AABB(staticCenter - rvec, staticCenter + rvec);
	}

	//Moving sphere
	Sphere (const Point3& center1, const Point3& center2, double radius, shared_ptr<Material> mat) 
		: center(center1, center2 - center1), radius(std::fmax(0, radius)), mat(mat) {
		Vector3 rvec = Vector3(radius, radius, radius);
		AABB box1(center.at(0) - rvec, center.at(0) + rvec);
		AABB box2(center.at(1) - rvec, center.at(1) + rvec);
		bbox = AABB(box1, box2);
	}

	bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override {
		Point3 currentCenter = center.at(r.time());
		Vector3 oc = currentCenter - r.origin();

		double a = r.direction().length_squared();
		double h = dot(r.direction(), oc);
		double c = oc.length_squared() - radius * radius;
		double discriminant = h * h - a * c;

		if (discriminant < 0) return false;

		double sqrtd = std::sqrt(discriminant);

		//Find the nearest root that lies in the acceptable range
		double root = (h - sqrtd) / a;

		if (!rayT.surrounds(root)) {
			root = (h + sqrtd) / a;

			if (!rayT.surrounds(root)) return false;
		}

		rec.t = root;
		rec.p = r.at(rec.t);

		Vector3 outwardNormal = (rec.p - currentCenter) / radius;
		rec.set_face_normal(r, outwardNormal);
		rec.mat = mat;

		get_sphere_uv(outwardNormal, rec.u, rec.v);

		return true;
	}

	AABB bounding_box() const override {
		return bbox;
	}
};

#endif