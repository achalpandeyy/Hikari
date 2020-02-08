#define _USE_MATH_DEFINES

#include "PathIntegrator.h"
#include "Sampler.h"

#include <cmath>

#define BLACK glm::vec3(0.f)

namespace Hikari
{
    glm::vec3 UniformSampleHemisphere(const glm::vec2& sample)
    {
        float sinTheta = std::sqrt(std::max(0.f, 1.f - sample[0] * sample[0]));
        return glm::vec3(sinTheta * std::cos(2.f * M_PI * sample[1]), sample[0],
            sinTheta * std::sin(2.f * M_PI * sample[1]));
    }

    void OrthonormalBasis(const glm::vec3& axisY, glm::vec3& axisX, glm::vec3& axisZ)
    {
        if (axisY.x < axisY.y)
            axisX = glm::normalize(glm::vec3(0.f, -axisY.z, axisY.y));
        else
            axisX = glm::normalize(glm::vec3(axisY.z, 0.f, -axisY.x));

        axisZ = glm::cross(axisX, axisY);
    }

    const unsigned int numBounces = 5u;
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

            // accumulatedRadiance += throughput * interaction.m_Shape->m_Emission;

            // Next Event Estimation
            //
            for (auto light : scene->m_Lights)
            {
                Ray lightRay = light->GetLightRay(interaction);

                if (scene->Occluded(lightRay))
                    continue;

                // TODO(achal): Can't the whole calculation of the `diffuse` component
                // be delegated to Light?
                //
                glm::vec3 diffuse = (interaction.m_Shape->m_Albedo / glm::vec3(M_PI))
                    * (light->GetIncidentLight(interaction.m_HitPoint))
                    * std::max(0.f, glm::dot(lightRay.m_Direction, interaction.m_Normal));

                accumulatedRadiance += throughput * diffuse;
            }

            glm::vec3 normalY = interaction.m_Normal;
            glm::vec3 normalX, normalZ;
            OrthonormalBasis(normalY, normalX, normalZ);

            glm::mat4 mat(glm::vec4(normalX, 0.f), glm::vec4(normalY, 0.f), glm::vec4(normalZ, 0.f),
                glm::vec4(glm::vec3(0.f), 1.f));
            Transform toShading(mat);

            const glm::vec2 sample = sampler.GetSample();

            glm::vec3 scatterDirection = toShading.TransformVector(UniformSampleHemisphere(sample));

            const float bias = 1e-3f;
            tracingRay.m_Origin = interaction.m_HitPoint + bias * interaction.m_Normal;
            tracingRay.m_Direction = glm::normalize(scatterDirection);

            float cosThetaI = std::abs(glm::dot(interaction.m_Normal, scatterDirection));
            throughput *= cosThetaI * interaction.m_Shape->m_Albedo;
        }
        return accumulatedRadiance;
    }

}   // namespace Hikari