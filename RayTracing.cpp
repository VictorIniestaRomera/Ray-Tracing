#include "ray_tracer.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

int main() {
	//World
	HittableList world;
	world.add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
	world.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

	//Camera
	Camera cam;
	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 500;
	cam.samplesPerPixel = 100;
	cam.maxDepth = 50;

	cam.render(world);

	return 0;
}