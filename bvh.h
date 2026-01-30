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
		bbox = AABB::EMPTY;

		for (size_t objectIndex = start; objectIndex < end; objectIndex++) {
			bbox = AABB(bbox, objects[objectIndex]->bounding_box());
		}

		int axis = bbox.longest_axis();

		auto comparator =	(axis == 0) ? box_x_compare :				//<- comparator is a pointer to function position
							(axis == 1) ? box_y_compare : box_z_compare;//	 to call later

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

	static bool box_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axisIndex) {
		Interval aAxisInterval = a->bounding_box().axis_interval(axisIndex);
		Interval bAxisInterval = b->bounding_box().axis_interval(axisIndex);

		return aAxisInterval.min < bAxisInterval.min;
	}

	static bool box_x_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
		return box_compare(a, b, 0);
	}

	static bool box_y_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
		return box_compare(a, b, 1);
	}

	static bool box_z_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) {
		return box_compare(a, b, 2);
	}
};

#endif