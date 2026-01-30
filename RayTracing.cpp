#include "ray_tracer.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "bvh.h"

int main() {
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

	return 0;
}