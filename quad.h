#ifndef QUAD_H
#define QUAD_H

#include "hittable.h"

class Quad : public Hittable {
	Point3 q;
	Vector3 u, v;
	shared_ptr<Material> mat;
	AABB b_box;

public:
	Quad(const Point3& q, const Vector3& u, const Vector3& v, shared_ptr<Material> mat) 
		: q(q), u(u), v(v), mat(mat) {
		set_bounding_box();
	}

	virtual void set_bounding_box() {
		AABB b_boxDiagonal1 = AABB(q, q + u + v);
		AABB b_boxDiagonal2 = AABB(q + u, q + v);

		b_box = AABB(b_boxDiagonal1, b_boxDiagonal2);
	}

	AABB bounding_box() const override {
		return b_box;
	}

	bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
		return false;	//To be implemented
	}
};

#endif