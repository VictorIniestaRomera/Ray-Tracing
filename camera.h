#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class Camera {
	int imageHeight;
	double pixelSamplesScale;
	Point3 cameraCenter;
	Point3 pixel100Loc;
	Vector3 pixelDeltaU;
	Vector3 pixelDeltaV;
	Vector3 u, v, w;
	Vector3 defocusDiskU;
	Vector3 defocusDiskV;

	void initialize() {
		imageHeight = int(imageWidth / aspectRatio);
		imageHeight = (imageHeight < 1) ? 1 : imageHeight;

		pixelSamplesScale = 1.0 / samplesPerPixel;

		cameraCenter = lookFrom;

		//Determine viewport dimensions
		double theta = degrees_to_radians(vFov);
		double h = std::tan(theta / 2);
		double viewportHeight = 2 * h * focusDistance;
		double viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);

		//Calculate u, v, w unit basis vectors for the camera coordinate frame
		w = unit_vector(lookFrom - lookAt);
		u = unit_vector(cross(vUp, w));
		v = cross(w, u);

		//Calculate Vectors across the horizontal and vertical viewport edges
		Vector3 viewportU = viewportWidth * u;
		Vector3 viewportV = viewportHeight * -v;

		//Calculate the horizontal and vertical delta vectors from pixel to pixel
		pixelDeltaU = viewportU / imageWidth;
		pixelDeltaV = viewportV / imageHeight;

		//Calculate the location of upper left pixel
		Vector3 viewportUpperLeft = cameraCenter - (focusDistance * w) - viewportU / 2 - viewportV / 2;
		pixel100Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

		//Calculate the camera defocus disk basis vectors
		double defocusRadius = focusDistance * std::tan(degrees_to_radians(defocusAngle / 2));
		defocusDiskU = u * defocusRadius;
		defocusDiskV = v * defocusRadius;
	}

	Ray get_ray(int k, int i) const {
		Vector3 offset = sample_square();
		Point3 pixelSample = pixel100Loc + ((k + offset.x()) * pixelDeltaU) + ((i + offset.y()) * pixelDeltaV);
		Point3 rayOrigin = (defocusAngle <= 0) ? cameraCenter : defocus_disk_sample();
		Vector3 rayDirection = pixelSample - rayOrigin;
		double rayTime = random_double();

		return Ray(rayOrigin, rayDirection, rayTime);
	}

	Vector3 sample_square() const {
		return Vector3(random_double() - 0.5, random_double() - 0.5, 0);
	}

	Point3 defocus_disk_sample() const {
		//Returns a random point in the camera defocus disk
		Vector3 p = random_in_unit_disk();

		return cameraCenter + (p[0] * defocusDiskU) + (p[1] * defocusDiskV);
	}

	Color ray_color(const Ray& r, int depth, const Hittable& world) {
		//Check if we've exceeded the ray limit
		if (depth <= 0) return Color(0, 0, 0);

		HitRecord rec;

		if (world.hit(r, Interval(0.001, INFINIT), rec)) {
			Ray scattered;
			Color attenuation;

			if (rec.mat->scatter(r, rec, attenuation, scattered)) 
				return attenuation * ray_color(scattered, depth - 1, world);

			return Color(0, 0, 0);
		}

		Vector3 unitDirection = unit_vector(r.direction());
		double a = 0.5 * (unitDirection.y() + 1.0);

		return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
	}

public:
	double aspectRatio = 1.0;			//Ratio of image width over height
	int imageWidth = 100;				//Rendered image width in pixel count
	int samplesPerPixel = 10;			//Count of random samples fer each pixel
	int maxDepth = 10;					//Maximun number of rays bouncing in scene
	double vFov = 90.0;					//Vertical view angle
	Point3 lookFrom = Point3(0, 0, 0);	//Point where the camera is looking from
	Point3 lookAt = Point3(0, 0, -1);	//Direction where the camera is looking
	Vector3 vUp = Vector3(0, 1, 0);		//Camera relative up direction
	double defocusAngle = 0.0;			//Variation angle of rays through each pixel
	double focusDistance = 10.0;		//Distane from camera lookform point to plane of perfect focus

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