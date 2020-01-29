#pragma once

#include "Scene.h"

#include <glm/glm.hpp>

#include <memory>

namespace Hikari
{

    class Ray;

    class Integrator
    {
    public:
        Integrator() {}
        virtual ~Integrator() {}

        virtual glm::vec3 Li(const Ray& ray, const std::shared_ptr<Scene>& scene) const = 0;
    };

}   // namespace Rays