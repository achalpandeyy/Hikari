#pragma once

#include "Core/Camera.h"
#include "Core/Integrator.h"

#include <glm/glm.hpp>

#include <memory>

namespace Hikari
{
    class Sampler;
    class Scene;

    class PathIntegrator : public SamplerIntegrator
    {
    public:
        PathIntegrator(unsigned int maxDepth, std::shared_ptr<const Camera> camera)
            : SamplerIntegrator(maxDepth, camera)
        {}

        glm::vec3 Li(const Ray& ray, const Scene& scene, Sampler& sampler) const override;
    };

}   // namespace Hikari
