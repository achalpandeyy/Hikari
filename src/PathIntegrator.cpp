#define _USE_MATH_DEFINES

#include "PathIntegrator.h"

#include <cmath>
#include <random>

#define BLACK glm::vec3(0.f)

namespace Hikari
{
    glm::vec3 UniformSampleHemisphere(float eps0, float eps1)
    {
        float sinTheta = std::sqrt(std::max(0.f, 1.f - eps0 * eps0));
        return glm::vec3(sinTheta * std::cos(2.f * M_PI * eps1), eps0, sinTheta * std::sin(2.f * M_PI * eps1));
    }

    // The param `axisX` should be normalized.
    void OrthonormalBasis(const glm::vec3& axisX, glm::vec3& axisY, glm::vec3& axisZ)
    {
        if (axisX.x < axisX.y)
            axisY = glm::normalize(glm::vec3(0.f, -axisX.z, axisX.y));
        else
            axisY = glm::normalize(glm::vec3(-axisX.z, 0.f, axisX.x));

        axisZ = glm::cross(axisX, axisY);
    }

    glm::vec3 Trace(const Ray& ray, const std::shared_ptr<Scene>& scene, unsigned int depth)
    {
        if (depth > 5u)
            return BLACK;

        Interaction interaction = scene->Intersect(ray);
        if (!interaction.m_Shape)
            return glm::vec3(0.f);

        glm::vec3 normalX = glm::normalize(interaction.m_Normal);
        glm::vec3 normalY, normalZ;
        OrthonormalBasis(normalX, normalY, normalZ);

        glm::mat4 mat(glm::vec4(normalX, 0.f), glm::vec4(normalY, 0.f), glm::vec4(normalZ, 0.f),
            glm::vec4(glm::vec3(0.f), 1.f));
        Transform worldToShading(glm::transpose(mat));

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_real_distribution<float> dist(0.f, 1.f);
        float eps0 = dist(rng), eps1 = dist(rng);

        glm::vec3 scatterDirection = worldToShading.TransformVector(UniformSampleHemisphere(eps0, eps1));

        const float bias = 1e-3f;
        Ray tracingRay(interaction.m_HitPoint + bias * interaction.m_Normal, glm::normalize(scatterDirection));

        float cosThetaI = glm::dot(interaction.m_Normal, scatterDirection);

        return interaction.m_Shape->m_Emission + Trace(tracingRay, scene, depth + 1) * cosThetaI * interaction.m_Shape->m_Albedo;
    }

    const unsigned int numBounces = 5u;
    glm::vec3 PathIntegrator::Li(const Ray& ray, const std::shared_ptr<Scene>& scene) const
    {
        return Trace(ray, scene, 0u);

        /*
        glm::vec3 accumulatedRadiance = BLACK;
        glm::vec3 throughput(0.f);
        for (unsigned int i = 0; i < numBounces; ++i)
        {
            Interaction interaction = scene->Intersect(ray);
            if (!interaction.m_Shape)
            {
                return accumulatedRadiance;
            }

            accumulatedRadiance += throughput;
            accumulatedRadiance += throughput * interaction.m_Shape->m_Emission;

            glm::vec3 normalX = interaction.m_Normal;
            glm::vec3 normalY, normalZ;
            OrthonormalBasis(normalX, normalY, normalZ);

            glm::mat4 mat(glm::vec4(normalX, 0.f), glm::vec4(normalY, 0.f), glm::vec4(normalZ, 0.f),
                glm::vec4(glm::vec3(0.f), 1.f));
            Transform worldToShading(glm::transpose(mat));

            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_real_distribution<float> dist(0.f, 1.f);
            float eps0 = dist(rng), eps1 = dist(rng);

            glm::vec3 scatterDirection = worldToShading.TransformVector(UniformSampleHemisphere(eps0, eps1));

            const float bias = 1e-3f;
            tracingRay.m_Origin = interaction.m_HitPoint + bias * interaction.m_Normal;
            tracingRay.m_Direction = glm::normalize(scatterDirection);

            float cosThetaI = std::abs(glm::dot(interaction.m_Normal, scatterDirection));
            throughput = cosThetaI * interaction.m_Shape->m_Albedo;

        }
        return accumulatedRadiance;
        */
    }

}   // namespace Hikari