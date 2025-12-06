#pragma once
#include "Object.h"
#include "Material.h"
#include <memory>

class Sphere : public Object
{
public:
	Sphere() = default;

	// existing convenience ctor: builds a Lambertian from color
	Sphere(const glm::vec3& position, float radius, const color3_t& color)
		: Object(Transform{ position }, std::make_shared<Lambertian>(color)),
		  position(position),
		  radius(radius)
	{ }

	// new ctor: accept any material instance
	Sphere(const glm::vec3& position, float radius, std::shared_ptr<Material> material)
		: Object(Transform{ position }, material),
		  position(position),
		  radius(radius)
	{ }

	bool Hit(const Ray& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit) override;

public:
	glm::vec3 position;
	float radius{ 0 };
};