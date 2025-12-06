#pragma once
#include "Color.h"     // ensure color3_t is available
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
    inline glm::vec3 at(float t) const noexcept
    {
        return origin + t * direction;
    }
};

struct raycastHit_t
{
    glm::vec3 point{ 0.0f };
    glm::vec3 normal{ 0.0f };
    float distance{ 0.0f };
    color3_t color{ 1.0f };
};