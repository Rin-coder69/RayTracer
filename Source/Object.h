#pragma once
#include "Color.h"   // provides color3_t

// forward declarations to avoid pulling in Ray.h here
struct Ray;
struct raycastHit_t;

class Object
{
public:
    Object() = default;
    Object(const color3_t& color) : color{ color } {}

    virtual bool Hit(const Ray& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit) = 0;

protected:
    color3_t color;
};
