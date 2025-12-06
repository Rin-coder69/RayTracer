#include "Material.h"
#include "Random.h"
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <cmath>

// Lambertian (diffuse) scatter
bool Lambertian::Scatter(const Ray& incident, const raycastHit_t& raycastHit, color3_t& attenuation, Ray& scattered) const {
    // set scattered ray using random direction from normal, diffuse the outgoing ray
    scattered.origin = raycastHit.point;
    // random diffuse direction: normal + random point in unit sphere
    scattered.direction = glm::normalize(raycastHit.normal + random::inUnitSphere());

    attenuation = albedo;

    return true;
}

bool Metal::Scatter(const Ray& incident, const raycastHit_t& raycastHit, color3_t& attenuation, Ray& scattered) const {
    // reflect the (normalized) incident direction about the surface normal
    glm::vec3 reflected = glm::reflect(glm::normalize(incident.direction), raycastHit.normal);

    // set scattered ray from reflected ray + random point in sphere (fuzz = 0 no randomness, fuzz = 1 random reflected)
    scattered.origin = raycastHit.point;
    scattered.direction = glm::normalize(reflected + fuzz * random::inUnitSphere());

    attenuation = albedo;

    // check that reflected ray is going away from surface normal
    return glm::dot(scattered.direction, raycastHit.normal) > 0.0f;
}

// Schlick's approximation for Fresnel reflectance
static float Schlick(float cosine, float refractiveIndex) {
    float r0 = (1.0f - refractiveIndex) / (1.0f + refractiveIndex);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * std::pow((1.0f - cosine), 5.0f);
}

bool Dielectric::Scatter(const Ray& incident, const raycastHit_t& raycastHit, color3_t& attenuation, Ray& scattered) const {
    glm::vec3 outNormal;
    float ni_over_nt;
    float cosine;

    glm::vec3 rayDirection = glm::normalize(incident.direction);

    // determine if ray is entering or exiting
    if (glm::dot(rayDirection, raycastHit.normal) > 0.0f) {
        // ray exiting surface
        outNormal = -raycastHit.normal;
        ni_over_nt = refractiveIndex;
        cosine = refractiveIndex * glm::dot(rayDirection, raycastHit.normal);
    } else {
        // ray entering surface
        outNormal = raycastHit.normal;
        ni_over_nt = 1.0f / refractiveIndex;
        cosine = -glm::dot(rayDirection, raycastHit.normal);
    }

    glm::vec3 refracted = glm::refract(rayDirection, outNormal, ni_over_nt);
    float reflectProbability = 1.0f;

    if (glm::length(refracted) > 0.0f) {
        reflectProbability = Schlick(cosine, refractiveIndex);
    }

    glm::vec3 reflected = glm::reflect(rayDirection, raycastHit.normal);

    glm::vec3 chosenDir = (random::getReal() < reflectProbability) ? reflected : refracted;
    scattered = Ray{ raycastHit.point, glm::normalize(chosenDir) };

    attenuation = albedo; // tint

    return true;
}
