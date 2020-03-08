#pragma once

#include "Core/Integrator.h"

#include <memory>

namespace Hikari
{
    class Camera;
    class Ray;
    class Sampler;
    class Scene;

    class WhittedIntegrator : public SamplerIntegrator
    {
    public:
        WhittedIntegrator(unsigned int maxDepth, std::shared_ptr<const Camera> camera)
            : SamplerIntegrator(maxDepth, camera)
        {}

        glm::vec3 Li(const Ray& ray, const Scene& scene, Sampler& sampler) const override;
    };

}   // namespace Hikari