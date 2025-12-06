#pragma once
#include "Color.h"   
#include "Material.h"
#include "Transform.h"// provides color3_t
#include <memory>

// forward declarations to avoid pulling in Ray.h here
struct Ray;
struct raycastHit_t;

class Object
{
public:
    Object() = default;
    Object(const Transform& transform,std::shared_ptr<Material> material)
		: transform{ transform }, material{ material } {
	}
    //Object(const color3_t& color) : color{ color } {}
    virtual bool Hit(const Ray& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit) = 0;

protected:
	Transform transform;
    std::shared_ptr<Material> material;
    //color3_t color;
};
