#pragma once

#include "Core/Interaction.h"
#include "Core/Scene.h"

#include <glm/glm.hpp>

#include <memory>

namespace Hikari
{
    class Camera;
    class ImageBlock;
    class Ray;
    class Sampler;
    class Scene;

    class Integrator
    {
    public:
        virtual ~Integrator() {}

        virtual void Render(const Scene& scene) = 0;
    };

    class SamplerIntegrator : public Integrator
    {
    public:
        SamplerIntegrator(unsigned int maxDepth, std::shared_ptr<const Camera> camera)
            : m_MaxDepth(maxDepth), m_Camera(camera) {}

        void Render(const Scene& scene) override;

        virtual glm::vec3 Li(const Ray& ray, const Scene& scene, Sampler& sampler) const = 0;

    protected:
        std::shared_ptr<const Camera> m_Camera;
        const unsigned int m_MaxDepth;

    private:
        void RenderBlock(ImageBlock& block, const Scene& scene) const;
    };

    glm::vec3 UniformSampleAllLights(const Interaction& illumPoint, const Scene& scene, Sampler& sampler);

    glm::vec3 UniformSampleOneLight(const Interaction& illumPoint, const Scene& scene, Sampler& sampler);

    glm::vec3 EstimateDirect(
        const Interaction&  interaction,
        const Light&        light,
        const Scene&        scene,
        Sampler&            sampler);

}   // namespace Hikari