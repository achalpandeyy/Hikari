#pragma once

#include "Integrator.h"

namespace Hikari
{

class WhittedIntegrator : public Integrator
{
public:
    WhittedIntegrator() {}

    glm::vec3 Li(const Ray& ray, const Scene& scene) const;
};

}   // namespace Hikari