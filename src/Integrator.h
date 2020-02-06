#pragma once

#include "Scene.h"

#include <glm/glm.hpp>

#include <memory>

namespace Hikari
{
    class Ray;
    class Sampler;

    class Integrator
    {
    public:
        Integrator() {}
        virtual ~Integrator() {}

        virtual glm::vec3 Li(const Ray& ray, Sampler& sampler, const std::shared_ptr<Scene>& scene) const = 0;
    };

}   // namespace Rays