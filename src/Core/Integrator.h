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
        SamplerIntegrator(std::shared_ptr<const Camera> camera) : m_Camera(camera) {}

        void Render(const Scene& scene) override;

        virtual glm::vec3 Li(const Ray& ray, Sampler& sampler, const Scene& scene, unsigned int depth = 0) const = 0;

    protected:
        std::shared_ptr<const Camera> m_Camera;

    private:
        void RenderBlock(ImageBlock& block, const Scene& scene) const;
    };

    glm::vec3 UniformSampleAllLights(const Interaction& interaction, const Scene& scene, Sampler& sampler);

    glm::vec3 UniformSampleOneLight(const Interaction& interaction, const Scene& scene, Sampler& sampler);

    glm::vec3 EstimateDirect(
        const Interaction&  interaction,
        const Light&        light,
        const Scene&        scene,
        Sampler&            sampler);

}   // namespace Hikari