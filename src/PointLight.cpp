#define _USE_MATH_DEFINES

#include "PointLight.h"

#include "Interaction.h"
#include "Ray.h"

#include <glm/glm.hpp>

#include <cmath>

namespace Hikari
{
    glm::vec3 PointLight::GetIncidentLight(const glm::vec3& hitPoint) const
    {
        float distanceToLightSquared = glm::dot(m_Position - hitPoint, m_Position - hitPoint);
        if (distanceToLightSquared < 1e-6)
        {
            distanceToLightSquared = 1e-6;
        }
        return (m_Intensity) / glm::vec3(4.f * M_PI * distanceToLightSquared);
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