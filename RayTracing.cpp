#include "RayTracing.h"

Color ray_color(const Ray& r) {
	Vector3 unitDirection = unit_vector(r.direction());
	double a = 0.5 * (unitDirection.y() + 1.0);

	return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

int main() {
	//Image size
	double aspectRatio = 16.0 / 9.0;
	int imageWidth = 256;

	int imageHeight = int(imageWidth / aspectRatio);
	imageHeight = (imageHeight < 1) ? 1 : imageHeight;

	//Camera
	double focalLength = 1.0;
	double viewportHeight = 2.0;
	double viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);

	Point3 cameraCenter = Point3(0, 0, 0);

	//Calculate Vectors across the horizontal and vertical viewport edges
	Vector3 viewportU = Vector3(viewportWidth, 0, 0);
	Vector3 viewportV = Vector3(0, -viewportHeight, 0);

	//Calculate the horizontal and vertical delta vectors from pixel to pixel
	Vector3 pixelDeltaU = viewportU / imageWidth;
	Vector3 pixelDeltaV = viewportV / imageHeight;

	//Calculate the location of upper left pixel
	Vector3 viewportUpperLeft = cameraCenter - Vector3(0, 0, focalLength) - viewportU / 2 - viewportV / 2;
	Vector3 pixel100Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

	//Render

	//Print image info
	std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

	for (int i = 0; i < imageHeight; i++) {
		std::clog << "\rScanning progress... " << (imageHeight - i) << "\n" << std::flush;

		for (int k = 0; k < imageWidth; k++) {
			Point3 pixelCenter = pixel100Loc + (k * pixelDeltaU) + (i * pixelDeltaV);
			Vector3 rayDirection = pixelCenter - cameraCenter;

			Ray r(cameraCenter, rayDirection);

			Color pixelColor = ray_color(r);

			write_color(std::cout, pixelColor);
		}
	}

	std::clog << "\rDone! \n";

	return 0;
}