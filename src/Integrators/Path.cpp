#include "Path.h"

#include "Core/Sampler.h"
#include "Core/Scene.h"
#include "Math/Constants.h"
#include "Math/Ray.h"

namespace Hikari
{
    glm::vec3 PathIntegrator::Li(const Ray& ray, Sampler& sampler, const Scene& scene, unsigned int depth) const
    {
        Ray tracingRay = ray;

        glm::vec3 accumulatedRadiance(0.f);
        glm::vec3 throughput(1.f);
        for (unsigned int i = 0; i < depth; ++i)
        {
            Interaction interaction = scene.Intersect(tracingRay);

            if (!interaction.m_Primitive)
            {
                return accumulatedRadiance;
            }

            const std::unique_ptr<BSDF> bsdf = interaction.m_Primitive->m_Material->ComputeScatteringFunctions(interaction);
            if (!bsdf) continue;

            glm::vec3 wo = -tracingRay.m_Direction;

            // Sample Illumination from lights - direct lighting.
            //
            for (auto light : scene.m_Lights)
            {
                glm::vec3 wi;
                float lightPdf;
                VisibilityTester visibility;

                const glm::vec2 lightSample = sampler.GetSample2D();

                glm::vec3 Li = light->Sample_Li(interaction, lightSample, &wi, &lightPdf, &visibility);

                if (lightPdf > 0.f && Li != glm::vec3(0.f))
                {
                    glm::vec3 f = bsdf->Evaluate(wo, wi) * std::max(0.f, glm::dot(wi, interaction.m_Normal));
                    if (f != glm::vec3(0.f))
                    {
                        if (!visibility.Unoccluded(scene))
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

            const glm::vec2 bsdfSample = sampler.GetSample2D();

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