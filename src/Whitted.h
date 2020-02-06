#pragma once

#include "Integrator.h"

#include <memory>

namespace Hikari
{

    class WhittedIntegrator : public Integrator
    {
    public:
        glm::vec3 Li(const Ray& ray, Sampler& sampler, const std::shared_ptr<Scene>& scene) const override;
    };

}   // namespace Hikari