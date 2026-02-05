#include "ray_tracer.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "bvh.h"

void bouncing_spheres() {
	HittableList world;

	//Materials & World
	auto checker = make_shared<TextureChecker>(0.32, Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
	world.add(make_shared<Sphere>(Point3(0.0, -1000.0, 0.0), 1000.0, make_shared<Lambertian>(checker)));

	auto mat1 = make_shared<Dielectric>(1.5);
	world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, mat1));

	auto mat2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
	auto move = Point3(-4, 1, 0) + Vector3(0, random_double(0, 1), 0);
	world.add(make_shared<Sphere>(Point3(-4, 1, 0), move, 1.0, mat2));

	auto mat3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, mat3));

	world = HittableList(make_shared<BVH_node>(world));

	//Camera
	Camera cam;
	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 500;
	cam.samplesPerPixel = 100;
	cam.maxDepth = 50;

	cam.vFov = 20;
	cam.lookFrom = Point3(13, 2, 3);
	cam.lookAt = Point3(0, 0, 0);
	cam.vUp = Vector3(0, 1, 0);

	cam.defocusAngle = 0.6;
	cam.focusDistance = 10.0;

	cam.render(world);
}

void checkered_spheres() {
	HittableList world;

	auto checker = make_shared<TextureChecker>(0.32, Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
	world.add(make_shared<Sphere>(Point3(0, -10, 0), 10, make_shared<Lambertian>(checker)));
	world.add(make_shared<Sphere>(Point3(0, 10, 0), 10, make_shared<Lambertian>(checker)));

	//Camera
	Camera cam;
	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 400;
	cam.samplesPerPixel = 100;
	cam.maxDepth = 50;

	cam.vFov = 20;
	cam.lookFrom = Point3(13, 2, 3);
	cam.lookAt = Point3(0, 0, 0);
	cam.vUp = Vector3(0, 1, 0);

	cam.defocusAngle = 0;

	cam.render(world);
}

void earth() {
	auto earthTexture = make_shared<ImageTexture>("earthmap.jpg");
	auto earthSurface = make_shared<Lambertian>(earthTexture);
	auto globe = make_shared<Sphere>(Point3(0, 0, 0), 2, earthSurface);

	//Camera
	Camera cam;
	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 400;
	cam.samplesPerPixel = 100;
	cam.maxDepth = 50;

	cam.vFov = 20;
	cam.lookFrom = Point3(0, 0, 12);
	cam.lookAt = Point3(0, 0, 0);
	cam.vUp = Vector3(0, 1, 0);

	cam.defocusAngle = 0;

	cam.render(HittableList(globe));
}

void perlin_spheres() {
	HittableList world;

	auto perlTexture = make_shared<NoiseTexture>(4);
	world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(perlTexture)));
	world.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Lambertian>(perlTexture)));

	//Camera
	Camera cam;
	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 400;
	cam.samplesPerPixel = 100;
	cam.maxDepth = 50;

	cam.vFov = 20;
	cam.lookFrom = Point3(13, 2, 3);
	cam.lookAt = Point3(0, 0, 0);
	cam.vUp = Vector3(0, 1, 0);

	cam.defocusAngle = 0;

	cam.render(world);
}

int main() {
	switch (4) {
	case 1:
		bouncing_spheres();

		break;

	case 2:
		checkered_spheres();

		break;

	case 3:
		earth();

		break;

	case 4:
		perlin_spheres();

		break;
	}

	return 0;
}