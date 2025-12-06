#include "Scene.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Color.h"
#include "Ray.h"
#include "Object.h" 
#include <glm/glm.hpp>
#include <iostream>

void Scene::Render(Framebuffer& framebuffer, const Camera& camera) {
	// trace ray for every framebuffer pixel
	for (int y = 0; y < framebuffer.height; y++) {
		for (int x = 0; x < framebuffer.width; x++)	{
			// set pixel (x,y) coordinates)
			glm::vec2 pixel{ static_cast<float>(x), static_cast<float>(y) };
			// normalize (0 <-> 1) the pixel value
			glm::vec2 point = pixel / glm::vec2{ static_cast<float>(framebuffer.width), static_cast<float>(framebuffer.height) };
			// flip the y value (bottom = 0, top = 1)
			point.y = 1.0f - point.y;

			// get ray from camera
			Ray ray = camera.GetRay(point);

			// trace ray and convert to SDL color
			raycastHit_t raycastHit;
			color3_t color = Trace(ray, 0.001f, FLT_MAX, raycastHit);

			framebuffer.DrawPoint(x, y, ColorConvert(color));
		}
	}
}

color3_t Scene::Trace(const Ray& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit)
{
	bool rayHit = false;
	float closestDistance = maxDistance;

	// check if scene objects are hit by the ray
	for (auto& object : objects) {
		// when checking objects don't include objects farther than closest hit (starts at max distance)
		if (object->Hit(ray, minDistance, closestDistance, raycastHit)) {
			rayHit = true;
			// set closest distance to the raycast hit distance (only hit objects closer than closest distance)
			closestDistance = raycastHit.distance; // FIX: use the hit distance
		}
	}

	// check if ray hit object
	if (rayHit) {
		// direct assignment (works if color3_t == glm::vec3):
		// color3_t color = raycastHit.normal;

		// recommended: map normal from [-1,1] -> [0,1] so it's a valid color
		color3_t color = 0.5f * (raycastHit.normal + color3_t{ 1.0f });
		return color;
	}

	// draw sky colors based on the ray y position
	glm::vec3 direction = glm::normalize(ray.direction);
	// shift direction y from -1 <-> 1 to 0 <-> 1
	float t = (direction.y + 1) * 0.5f;

	// interpolate between sky bottom (0) to sky top (1)
	color3_t color = glm::mix(skyBottom, skyTop, t);

	return color;
}
void Scene::AddObject(std::unique_ptr<Object>object) {
	objects.push_back(std::move(object));
}

/*color3_t Scene::Trace(const Ray& ray) {
	// Sky gradient based on ray direction Y
	glm::vec3 direction = glm::normalize(ray.direction);

	// shift direction.y from [-1,1] to [0,1]
	float t = (direction.y + 1.0f) * 0.5f;

	// interpolate between skyBottom and skyTop
	color3_t color = glm::mix(skyBottom, skyTop, t);

	return color;
}*/


/*color3_t Scene::Trace(const ray_t& ray, float minDistance, float maxDistance) {

	bool rayHit = false;
	float closestDistance = maxDistance;
	raycastHit_t raycastHit;

	// check if scene objects are hit by the ray
	for (auto& object : objects) {
		// when checking objects don't include objects farther than closest hit (starts at max distance)
		if (object->Hit(ray, minDistance, closestDistance, raycastHit))	{
			rayHit = true;
			// set closest distance to the raycast hit distance (only hit objects closer than closest distance)
			closestDistance = // raycast hit distance;
		}
	}

	// check if ray hit object
	if (rayHit)	{
		// get material color of hit object
		color3_t color = raycastHit.color;
		return color;
	}

	// draw sky colors based on the ray y position
	glm::vec3 direction = glm::normalize(ray.direction);
	// shift direction y from -1 <-> 1 to 0 <-> 1
	float t = (direction.y + 1) * 0.5f;
	
	// interpolate between sky bottom (0) to sky top (1)
	color3_t color = glm::mix(skyBottom, skyTop, t);

	return color;
}
*/