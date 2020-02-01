#pragma once

#include "Integrator.h"

#include <glm/glm.hpp>

namespace Hikari
{

    class PathIntegrator : public Integrator
    {
    public:
        glm::vec3 Li(const Ray& ray, const std::shared_ptr<Scene>& scene) const override;;
    };

}   // namespace Hikari
