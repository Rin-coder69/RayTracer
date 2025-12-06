#include "Scene.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Color.h"
#include "Ray.h"
#include "Object.h"
#include <glm/glm.hpp>
#include <iostream>
#include <random>

void Scene::Render(Framebuffer& framebuffer, const Camera& camera, int numSamples) {
	// random number generator for sample jitter
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.0f, 1.0f);

	// trace ray for every framebuffer pixel
	for (int y = 0; y < framebuffer.height; y++) {
		for (int x = 0; x < framebuffer.width; x++) {
			// color will be accumulated with ray trace samples
			color3_t color{ 0 };
			// multi-sample for each pixel
			for (int i = 0; i < numSamples; i++) {
				// set pixel (x,y) coordinates
				glm::vec2 pixel{ static_cast<float>(x), static_cast<float>(y) };
				// add random value (0-1) to pixel value, each sample should be a little different
				pixel += glm::vec2{ dis(gen), dis(gen) };
				// normalize (0 <-> 1) the pixel value (pixel / vec2{ framebuffer.width, framebuffer.height })
				glm::vec2 point = pixel / glm::vec2{ static_cast<float>(framebuffer.width), static_cast<float>(framebuffer.height) };
				// flip the y value (bottom = 0, top = 1)
				point.y = 1.0f - point.y;

				// get ray from camera
				Ray ray = camera.GetRay(point);

				// trace ray (use a small minDistance to avoid self-intersection and a large maxDistance)
				color += Trace(ray, 0.001f, 1000.0f);
			}
			// get average color = (color / number samples)
			color = color / static_cast<float>(numSamples);
			framebuffer.DrawPoint(x, y, ColorConvert(color));
		}
	}
}

color3_t Scene::Trace(const Ray& ray, float minDistance, float maxDistance, int maxDepth)
{
	// reached max depth (bounce) return black color
	if (maxDepth <= 0) {
		return color3_t{ 0.0f, 0.0f, 0.0f };
	}

	bool rayHit = false;
	float closestDistance = maxDistance;
	raycastHit_t closestHit;

	// check if scene objects are hit by the ray
	for (const auto& object : objects) {
		raycastHit_t tempHit;
		// when checking objects don't include objects farther than closest hit (starts at max distance)
		if (object->Hit(ray, minDistance, closestDistance, tempHit)) {
			rayHit = true;
			closestDistance = tempHit.distance; // only consider hits closer than previous closest
			closestHit = tempHit; // store the closest hit
		}
	}

	// check if ray hit object
	if (rayHit && closestHit.material) {
		color3_t attenuation;
		Ray scattered;
		// get raycast hit material, get material color and scattered ray 
		if (closestHit.material->Scatter(ray, closestHit, attenuation, scattered)) {
			// trace scattered ray, final color will be the product of all the material colors
			return attenuation * Trace(scattered, minDistance, maxDistance, maxDepth - 1);
		}
		else {
			// material absorbed the ray (emissive)
			return closestHit.material->GetEmissive();
		}
	}

	// no ray hit, draw sky colors based on the ray y position
	glm::vec3 direction = glm::normalize(ray.direction);
	// shift direction y from -1 <-> 1 to 0 <-> 1
	float t = (direction.y + 1.0f) * 0.5f;

	// interpolate between sky bottom (0) to sky top (1)
	color3_t skyColor = glm::mix(skyBottom, skyTop, t);

	return skyColor;
}

void Scene::AddObject(std::unique_ptr<Object> object) {
	objects.push_back(std::move(object));
}