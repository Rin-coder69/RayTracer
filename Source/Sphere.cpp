#include "Sphere.h"
#include "Ray.h"
#include <glm/glm.hpp>
#include <cmath>
#include <limits>

bool Sphere::Hit(const Ray& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit) {
    // vector from sphere center to ray origin
    glm::vec3 oc = ray.origin - position;

    // quadratic coefficients for ray–sphere intersection
    float a = glm::dot(ray.direction, ray.direction);
    if (a <= std::numeric_limits<float>::epsilon()) {
        // degenerate ray (no direction)
        return false;
    }

    // use half_b = dot(direction, oc) for numerical stability
    float half_b = glm::dot(ray.direction, oc);
    float c = glm::dot(oc, oc) - radius * radius;

    // discriminant = half_b^2 - a*c
    float discriminant = half_b * half_b - a * c;
    if (discriminant < 0.0f) {
        return false;
    }

    float sqrtD = std::sqrt(discriminant);

    // smaller root (closest hit)
    float t = (-half_b - sqrtD) / a;
    if (t > minDistance && t < maxDistance) {
        raycastHit.distance = t;
        raycastHit.point = ray.at(t);
        raycastHit.normal = (raycastHit.point - position) / radius;

        // set raycastHit.color from normal (maps [-1,1] -> [0,1])
        raycastHit.color = 0.5f * (raycastHit.normal + color3_t{ 1.0f });
        return true;
    }

    // try the farther root
    t = (-half_b + sqrtD) / a;
    if (t > minDistance && t < maxDistance) {
        raycastHit.distance = t;
        raycastHit.point = ray.at(t);
        raycastHit.normal = (raycastHit.point - position) / radius;

        // set raycastHit.color from normal (maps [-1,1] -> [0,1])
        raycastHit.color = 0.5f * (raycastHit.normal + color3_t{ 1.0f });
        return true;
    }

    return false;
}
