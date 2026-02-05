#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

class PerlinNoise {
	static const int pointCount = 256;
	Vector3 randVec[pointCount];
	int perm_x[pointCount];
	int perm_y[pointCount];
	int perm_z[pointCount];

	static void perlin_generate_perm(int* p) {
		for (int i = 0; i < pointCount; i++) {
			p[i] = i;
		}

		permute(p, pointCount);
	}

	static void permute(int* p, int n) {
		for (int i = n - 1; i > 0; i--) {
			int target = random_int(0, i);
			int tmp = p[i];
			p[i] = p[target];
			p[target] = tmp;
		}
	}

	static double perlin_interp(const Vector3 c[2][2][2], double u, double v, double w) {
		double accum = 0.0;

		double uu = u * u * (3 - 2 * u);
		double vv = v * v * (3 - 2 * v);
		double ww = w * w * (3 - 2 * w);

		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				for (int k = 0; k < 2; k++) {
					Vector3 weight_v(u - i, v - j, w - k);

					accum +=	(i * uu + (1 - i) * (1 - uu))	*
								(j * vv + (1 - j) * (1 - vv))	*
								(k * ww + (1 - k) * (1 - ww))	*
								dot(c[i][j][k], weight_v);
				}
			}
		}

		return accum;
	}

public:
	PerlinNoise() {
		for (int i = 0; i < pointCount; i++) {
			randVec[i] = unit_vector(Vector3::random(-1, 1));
		}

		perlin_generate_perm(perm_x);
		perlin_generate_perm(perm_y);
		perlin_generate_perm(perm_z);
	}

	double noise(const Point3& p) const {
		double u = p.x() - std::floor(p.x());
		double v = p.y() - std::floor(p.y());
		double w = p.z() - std::floor(p.z());

		int i = int(std::floor(p.x()));
		int j = int(std::floor(p.y()));
		int k = int(std::floor(p.z()));

		Vector3 c[2][2][2];

		for (int di = 0; di < 2; di++) {
			for (int dj = 0; dj < 2; dj++) {
				for (int dk = 0; dk < 2; dk++) {
					c[di][dj][dk] = randVec[
						perm_x[(i + di) & 255] ^
						perm_y[(j + dj) & 255] ^
						perm_z[(k + dk) & 255]];
				}
			}
		}

		return perlin_interp(c, u, v, w);
	}

	double turb(const Point3& p, int depth) const {
		double accum = 0.0;
		double weight = 1.0;
		Point3 temp_p = p;

		for (int i = 0; i < depth; i++) {
			accum += weight * noise(temp_p);
			weight *= 0.5f;
			temp_p *= 2;
		}

		return std::fabs(accum);
	}
};

#endif