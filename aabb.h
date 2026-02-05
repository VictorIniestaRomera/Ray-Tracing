#ifndef AABB_H
#define AABB_H

class AABB {
	void pad_to_minus() {
		double delta = 0.0001;

		if (x.size() < delta) x = x.expand(delta);

		if (y.size() < delta) y = y.expand(delta);

		if (z.size() < delta) z = z.expand(delta);
	}

public:
	Interval x, y, z;

	static const AABB EMPTY, UNIVERSE;

	AABB() {}

	AABB(const Interval& x, const Interval& y, const Interval& z) : x(x), y(y), z(z) {
		pad_to_minus();
	}

	AABB(const Point3& a, const Point3 b) {
		x = (a[0] <= b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
		y = (a[1] <= b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
		z = (a[2] <= b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);

		pad_to_minus();
	}

	AABB(const AABB& box1, const AABB& box2) {
		x = Interval(box1.x, box2.x);
		y = Interval(box1.y, box2.y);
		z = Interval(box1.z, box2.z);
	}

	const Interval& axis_interval(int n) const {
		return (n == 1) ? y : (n == 2) ? z : x;
	}

	bool hit(const Ray& r, Interval rayT) const {
		const Point3& rayOrig = r.origin();
		const Vector3& rayDir = r.direction();

		for (int axis = 0; axis < 3; axis++) {
			const Interval& ax = axis_interval(axis);
			const double adinv = 1.0 / rayDir[axis];

			double t0 = (ax.min - rayOrig[axis]) * adinv;
			double t1 = (ax.max - rayOrig[axis]) * adinv;

			if (t0 < t1) {
				if (t0 > rayT.min) rayT.min = t0;

				if (t1 < rayT.max) rayT.max = t1;
			}
			else {
				if (t1 > rayT.min) rayT.min = t1;

				if (t0 < rayT.max) rayT.max = t0;
			}

			if (rayT.max <= rayT.min) return false;
		}

		return true;
	}

	int longest_axis() const {
		if (x.size() > y.size()) return (x.size() > z.size()) ? 0 : 2;
		else return (y.size() > z.size()) ? 1 : 2;
	}
};

const AABB AABB::EMPTY		= AABB(Interval::EMPTY, Interval::EMPTY, Interval::EMPTY);
const AABB AABB::UNIVERSE	= AABB(Interval::UNIVERSE, Interval::UNIVERSE, Interval::UNIVERSE);

#endif