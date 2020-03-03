#define _USE_MATH_DEFINES

#include "Path.h"

#include "Math/Constants.h"
#include "Core/Sampler.h"

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

            glm::vec3 wo = -tracingRay.m_Direction;

            // Sample Illumination from lights - direct lighting.
            //
            for (auto light : scene->m_Lights)
            {
                glm::vec3 wi;
                float lightPdf;
                VisibilityTester visibility;

                const glm::vec2 lightSample = sampler.GetSample();

                glm::vec3 Li = light->Sample_Li(interaction, lightSample, &wi, &lightPdf, &visibility);

                if (lightPdf > 0.f && Li != glm::vec3(0.f))
                {
                    glm::vec3 f = bsdf->Evaluate(wo, wi) * std::max(0.f, glm::dot(wi, interaction.m_Normal));
                    if (f != glm::vec3(0.f))
                    {
                        if (!visibility.Unoccluded(*scene.get()))
                        {
                            Li = glm::vec3(0.f);
                        }
                        else
                        {
                            accumulatedRadiance += throughput * f * Li / lightPdf;
                        }
                    }
                }
            }

            // Sample bsdf

            glm::vec3 wi;
            float pdf;

            const glm::vec2 bsdfSample = sampler.GetSample();

            bsdf->Sample(wi, bsdfSample, pdf);
            if (pdf == 0.f) break;

            glm::vec3 f = bsdf->Evaluate(wo, wi);
            if (f == glm::vec3(0.f)) break;

            throughput *= f * std::abs(glm::dot(interaction.m_Normal, wi)) / pdf;

            tracingRay.m_Origin = interaction.m_Position + EPSILON * interaction.m_Normal;
            tracingRay.m_Direction = wi;
        }
        return accumulatedRadiance;
    }

}   // namespace Hikari