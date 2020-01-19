#pragma once

#include "Scene.h"

#include "glm/vec3.hpp"

#include <memory>

namespace Hikari
{

    class Ray;

    class Integrator
    {
    public:
        virtual ~Integrator() {}

        virtual glm::vec3 Li(const Ray& ray, const Scene&) const = 0;
    };

}   // namespace Rays