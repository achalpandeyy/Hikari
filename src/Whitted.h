#pragma once

#include "Integrator.h"

namespace Hikari
{

class WhittedIntegrator : public Integrator
{
public:
    glm::vec3 Li(const Ray& ray, const Scene& scene, unsigned int rayDepth) const;
};

}   // namespace Hikari