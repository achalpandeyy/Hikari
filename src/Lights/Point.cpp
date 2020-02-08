#include "Point.h"

#include "Interaction.h"
#include "Ray.h"

namespace Hikari
{
    glm::vec3 PointLight::GetIncidentRadiance(const glm::vec3& hitPoint) const
    {
        float distanceToLightSquared = glm::dot(m_Position - hitPoint, m_Position - hitPoint);
        if (distanceToLightSquared < 1e-8f)
        {
            distanceToLightSquared = 1e-8f;
        }
        return m_Intensity / distanceToLightSquared;
    }

    Ray PointLight::GetLightRay(const Interaction& interaction) const
    {
        // TODO(achal): Make the bias globally accessible
        const float bias = 1e-3f;

        glm::vec3 lightDirection = glm::normalize(m_Position - interaction.m_HitPoint);
        float distanceToLight = glm::distance(interaction.m_HitPoint, m_Position);
        return Ray(interaction.m_HitPoint + interaction.m_Normal * bias, lightDirection, distanceToLight);
    }

}   // namespace Hikari