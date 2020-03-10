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

        for (unsigned int bounces = 0u; ; ++bounces)
        {
            // Find the next path vertex and accumulate contribution
            //
            Interaction interaction;

            // Terminate path if ray escaped
            //
            if (!scene.Intersect(tracingRay, &interaction) || bounces >= m_MaxDepth)
                break;

            // Possibly add emitted light at the intersection point
            //
            if (bounces == 0u)
            {
                // Add emitted light at path vertex
                //
                if (interaction.m_Primitive)
                    L += throughput * interaction.Le();
            }

            // Compute scattering functions
            //
            interaction.ComputeScatteringFunctions();

            // Sample illumination from lights to find path contribution
            //
            L += throughput * UniformSampleOneLight(interaction, scene, sampler);

            // Sample BSDF to get new path direction
            //
            glm::vec3 wo = -tracingRay.m_Direction, wi;

            float pdf;

            glm::vec3 f = interaction.m_BSDF->Sample_f(wo, &wi, sampler.GetSample2D(), &pdf);

            if (f == glm::vec3(0.f) || pdf == 0.f)
                break;

            throughput *= f * glm::abs(glm::dot(wi, interaction.m_Normal)) / pdf;
            
            tracingRay = interaction.SpawnRay(wi);
        }

        return L;
    }

}   // namespace Hikari