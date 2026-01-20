#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class Camera {
	int imageHeight;
	double pixelSamplesScale;
	Point3 cameraCenter;
	Point3 pixel100Loc;
	Vector3 pixelDeltaU;
	Vector3 pixelDeltaV;

	void initialize() {
		imageHeight = int(imageWidth / aspectRatio);
		imageHeight = (imageHeight < 1) ? 1 : imageHeight;

		pixelSamplesScale = 1.0 / samplesPerPixel;

		cameraCenter = Point3(0, 0, 0);

		//Determine viewport dimensions
		double focalLength = 1.0;
		double viewportHeight = 2.0;
		double viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);

		//Calculate Vectors across the horizontal and vertical viewport edges
		Vector3 viewportU = Vector3(viewportWidth, 0, 0);
		Vector3 viewportV = Vector3(0, -viewportHeight, 0);

		//Calculate the horizontal and vertical delta vectors from pixel to pixel
		pixelDeltaU = viewportU / imageWidth;
		pixelDeltaV = viewportV / imageHeight;

		//Calculate the location of upper left pixel
		Vector3 viewportUpperLeft = cameraCenter - Vector3(0, 0, focalLength) - viewportU / 2 - viewportV / 2;
		pixel100Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
	}

	Ray get_ray(int k, int i) const {
		Vector3 offset = sample_square();
		Point3 pixelSample = pixel100Loc + ((k + offset.x()) * pixelDeltaU) + ((i + offset.y()) * pixelDeltaV);
		Point3 rayOrigin = cameraCenter;
		Vector3 rayDirection = pixelSample - rayOrigin;

		return Ray(rayOrigin, rayDirection);
	}

	Vector3 sample_square() const {
		return Vector3(random_double() - 0.5, random_double() - 0.5, 0);
	}

	Color ray_color(const Ray& r, int depth, const Hittable& world) {
		//Check if we've exceeded the ray limit
		if (depth <= 0) return Color(0, 0, 0);

		HitRecord rec;

		if (world.hit(r, Interval(0.001, INFINIT), rec)) {
			Vector3 direction = rec.normal + random_unit_vector();

			return 0.5 * ray_color(Ray(rec.p, direction), depth - 1, world);
		}

		Vector3 unitDirection = unit_vector(r.direction());
		double a = 0.5 * (unitDirection.y() + 1.0);

		return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
	}

public:
	double aspectRatio = 1.0;	//Ratio of image width over height
	int imageWidth = 100;		//Rendered image width in pixel count
	int samplesPerPixel = 10;	//Count of random samples fer each pixel
	int maxDepth = 10;			//Maximun number of rays bouncing in scene

	void render(const Hittable& world) {
		initialize();

		//Print image info
		std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

		for (int i = 0; i < imageHeight; i++) {
			std::clog << "\rScanning progress... " << (imageHeight - i) << std::flush;

			for (int k = 0; k < imageWidth; k++) {
				Color pixelColor(0, 0, 0);

				for (int sample = 0; sample < samplesPerPixel; sample++) {
					Ray r = get_ray(k, i);

					pixelColor += ray_color(r, maxDepth, world);
				}

				write_color(std::cout, pixelSamplesScale * pixelColor);
			}
		}

		std::clog << "\rDone! \n";
	}
};

#endif