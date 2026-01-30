#ifndef BVH_H
#define BVH_H

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"

#include <algorithm>

class BVH_node : public Hittable {
	shared_ptr<Hittable> left;
	shared_ptr<Hittable> right;
	AABB bbox;

public:
	BVH_node(HittableList list) : BVH_node(list.objects, 0, list.objects.size()) {}

	BVH_node(std::vector<shared_ptr<Hittable>>& objects, size_t start, size_t end) {
		int axis = random_int(0, 2);

		auto comparator = 0;	//Change

		size_t objectSpan = end - start;
		
		if (objectSpan == 1)
			left = right = objects[start];	//Both left and right have been given the value of objects[start]
		else if (objectSpan == 2) {
			left = objects[start];
			right = objects[start + 1];
		}
		else {
			std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

			size_t mid = start + objectSpan / 2;
			left = make_shared<BVH_node>(objects, start, mid);
			right = make_shared<BVH_node>(objects, mid, end);
		}

		bbox = AABB(left->bounding_box(), right->bounding_box());
	}

	bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override {
		if (!bbox.hit(r, rayT)) return false;

		bool hitLeft = left->hit(r, rayT, rec);
		bool hitRight = right->hit(r, Interval(rayT.min, (hitLeft) ? rec.t : rayT.max), rec);

		return hitLeft || hitRight;	//If either hitLeft or hitRight is true, returns true
	}

	AABB bounding_box() const override {
		return bbox;
	}
};

#endif