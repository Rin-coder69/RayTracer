#pragma once 
#include <glm/vec3.hpp>

struct Ray
{
	glm::vec3 origin{ 0.0f };
	glm::vec3 direction{ 0.0f };

	Ray() = default;
	Ray(const glm::vec3& origin, const glm::vec3& direction)
		: origin(origin), direction(direction)
	{
	}
	inline glm::vec3 at(float t)const noexcept
	{
		return origin + t * direction;
	}

	//inline glm::vec3 operator*(const Ray& r, float t) noexcept { return r.at(t); }
	//inline glm::vec3 operator*(float t, const Ray& r) noexcept { return r.at(t); }
};