#include "Spot.h"

#include "Core/Interaction.h"
#include "Math/Ray.h"

namespace Hikari
{
    glm::vec3 SpotLight::GetIncidentRadiance(const glm::vec3& hitPoint) const
    {
        glm::vec3 lightDirection = glm::normalize(m_Position - hitPoint);
        float distanceToLightSquared = glm::dot(m_Position - hitPoint, m_Position - hitPoint);
        if (distanceToLightSquared < 1e-8f)
        {
            distanceToLightSquared = 1e-8f;
        }
        return (m_Intensity * Falloff(-lightDirection)) / distanceToLightSquared;
    }

    Ray SpotLight::GetLightRay(const Interaction& interaction) const
    {
        const float bias = 1e-3f;

        glm::vec3 lightDirection = glm::normalize(m_Position - interaction.m_HitPoint);
        float distanceToLight = glm::distance(interaction.m_HitPoint, m_Position);
        return Ray(interaction.m_HitPoint + interaction.m_Normal * bias, lightDirection, distanceToLight);
    }

    float SpotLight::Falloff(const glm::vec3& direction) const
    {
        float cosTheta = glm::dot(direction, m_RefDirection);

        if (cosTheta < m_CosTotalWidth) return 0.f;
        if (cosTheta > m_CosFalloffStart) return 1.f;

        float delta = (cosTheta - m_CosTotalWidth) / (m_CosFalloffStart - m_CosTotalWidth);
        return (delta * delta) * (delta * delta);
    }

}   // namespace Hikari