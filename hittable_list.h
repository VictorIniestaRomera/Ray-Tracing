#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>

#include "hittable.h"
#include "aabb.h"

class HittableList : public Hittable {
	AABB bbox;

public:
	std::vector<shared_ptr<Hittable>> objects;
	
	HittableList() {}

	HittableList(shared_ptr<Hittable> object) {
		add(object);
	}

	void clear() {
		objects.clear();
	}

	void add(shared_ptr<Hittable> object) {
		objects.push_back(object);
		bbox = AABB(bbox, object->bounding_box());
	}

	bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override {
		HitRecord tempRec;
		bool hitAnything = false;
		double closestSoFar = rayT.max;

		for (const auto& object : objects) {
			if (object->hit(r, Interval(rayT.min, closestSoFar), tempRec)) {
				hitAnything = true;
				closestSoFar = tempRec.t;
				rec = tempRec;
			}
		}

		return hitAnything;
	}

	AABB bounding_box() const override {
		return bbox;
	}
};

#endif