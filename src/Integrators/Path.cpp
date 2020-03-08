#include "Path.h"

#include "Core/BSDF.h"
#include "Core/Interaction.h"
#include "Core/Sampler.h"
#include "Core/Scene.h"
#include "Math/Constants.h"
#include "Math/Ray.h"

namespace Hikari
{
    glm::vec3 PathIntegrator::Li(const Ray& ray, const Scene& scene, Sampler& sampler) const
    {
        glm::vec3 L(0.f), throughput(1.f);

        Ray tracingRay(ray);

        for (unsigned int bounces = 0u; bounces <= m_MaxDepth; ++bounces)
        {
            // Find the next path vertex and accumulate contribution

            // Intersect the ray with the scene and store the result in an Interaction
            //
            Interaction interaction = scene.Intersect(tracingRay);

            // Terminate path if ray escaped
            //
            if (!interaction.m_Primitive)
                break;

            // Possibly add emitted light at the intersection point
            //
            if (bounces == 0u)
            {
                // Add emitted light at path vertex or from the environment
                if (!interaction.m_Primitive)
                    L += throughput * interaction.Le();
            }

            // Compute scattering functions
            //
            std::unique_ptr<BSDF> bsdf = interaction.m_Primitive->m_Material->ComputeScatteringFunctions(interaction);

            // Sample illumination from lights to find path contribution
            //
            L += throughput * UniformSampleOneLight(interaction, scene, sampler);

            // Sample BSDF to get new path direction
            //
            glm::vec3 wo = -tracingRay.m_Direction, wi;
            float pdf;

            // TODO(achal): Combine the two following functions into one -- BSDF::Sample_f
            bsdf->Sample(wi, sampler.GetSample2D(), pdf);
            glm::vec3 f = bsdf->Evaluate(wo, wi);

            if (f == glm::vec3(0.f) || pdf == 0.f)
                break;

            throughput *= f * glm::abs(glm::dot(wi, interaction.m_Normal)) / pdf;
            
            tracingRay = interaction.SpawnRay(wi);
        }

        return L;
    }

}   // namespace Hikari