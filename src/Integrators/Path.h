#pragma once

#include "Core/Camera.h"
#include "Core/Integrator.h"

#include <glm/glm.hpp>

namespace Hikari
{
    class PathIntegrator : public SamplerIntegrator
    {
    public:
        PathIntegrator(std::shared_ptr<const Camera> camera) : SamplerIntegrator(camera) {}

        glm::vec3 Li(const Ray& ray, Sampler& sampler, const Scene& scene, unsigned int depth = 0) const override;
    };

}   // namespace Hikari
