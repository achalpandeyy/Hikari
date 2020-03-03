#pragma once

#include "Core/Integrator.h"

#include <glm/glm.hpp>

namespace Hikari
{

    class PathIntegrator : public Integrator
    {
    public:
        glm::vec3 Li(const Ray& ray, Sampler& sampler, const std::shared_ptr<Scene>& scene) const override;;
    };

}   // namespace Hikari
