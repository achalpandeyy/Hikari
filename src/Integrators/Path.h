#pragma once

#include "Core/Camera.h"
#include "Core/Integrator.h"

#include <glm/glm.hpp>

namespace Hikari
{
    class Sampler;
    class Scene;

    class PathIntegrator : public SamplerIntegrator
    {
    public:
        PathIntegrator(std::shared_ptr<const Camera> camera) : SamplerIntegrator(camera) {}

        glm::vec3 Li(const Ray& ray, const Scene& scene, Sampler& sampler, unsigned int depth = 0) const override;
    };

}   // namespace Hikari
