#define _USE_MATH_DEFINES

#include "Core/Constants.h"
#include "PathIntegrator.h"
#include "Sampler.h"

#include <cmath>

#define BLACK glm::vec3(0.f)

namespace Hikari
{
    const unsigned int numBounces = 50u;

    glm::vec3 PathIntegrator::Li(const Ray& ray, Sampler& sampler, const std::shared_ptr<Scene>& scene) const
    {
        Ray tracingRay = ray;

        glm::vec3 accumulatedRadiance = BLACK;
        glm::vec3 throughput(1.f);
        for (unsigned int i = 0; i < numBounces; ++i)
        {
            Interaction interaction = scene->Intersect(tracingRay);

            if (!interaction.m_Shape)
            {
                return accumulatedRadiance;
            }

            const std::unique_ptr<BSDF> bsdf = interaction.m_Shape->m_Material->ComputeScatteringFunctions(interaction);
            if (!bsdf) continue;
            
            const glm::vec2 sample = sampler.GetSample();

            glm::vec3 wo = -tracingRay.m_Direction, wi;
            float pdf;

            bsdf->Sample(wi, sample, pdf);
            if (pdf == 0.f) break;

            glm::vec3 f = bsdf->Evaluate(wo, wi);
            if (f == glm::vec3(0.f)) break;

            // Sample Illumination from lights - direct lighting.
            //
            for (auto light : scene->m_Lights)
            {
                Ray lightRay = light->GetLightRay(interaction);

                if (scene->Occluded(lightRay))
                    continue;

                accumulatedRadiance += f * throughput * light->GetIncidentRadiance(interaction.m_HitPoint) * std::max(0.f, glm::dot(lightRay.m_Direction, interaction.m_Normal));
            }

            throughput *= f * std::abs(glm::dot(interaction.m_Normal, wi)) / pdf;

            tracingRay.m_Origin = interaction.m_HitPoint + EPSILON * interaction.m_Normal;
            tracingRay.m_Direction = wi;
        }
        return accumulatedRadiance;
    }

}   // namespace Hikari