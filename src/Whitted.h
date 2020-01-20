#pragma once

#include "EmbreeRTEngine.h"
#include "Integrator.h"

#include <memory>

namespace Hikari
{

class WhittedIntegrator : public Integrator
{
public:
    WhittedIntegrator(std::shared_ptr<EmbreeRTEngine> rtEngine) 
        : Integrator(rtEngine)
    {}

    glm::vec3 Li(const Ray& ray, const Scene& scene) const;
};

}   // namespace Hikari