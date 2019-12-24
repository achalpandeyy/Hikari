#pragma once

#include "Scene.h"

#include "glm/vec3.hpp"

namespace Rays { class Ray; }

namespace Rays
{

class Integrator
{
public:
    virtual ~Integrator() {}

    virtual glm::vec3 Li(const Ray& ray, const Scene&, unsigned int rayDepth) const = 0;
};

}   // namespace Rays