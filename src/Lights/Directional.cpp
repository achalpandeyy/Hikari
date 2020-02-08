#include "Directional.h"

#include "Interaction.h"
#include "Ray.h"

namespace Hikari
{
    glm::vec3 DirectionalLight::GetIncidentRadiance(const glm::vec3& hitPoint) const
    {
        return m_Intensity;
    }

    Ray DirectionalLight::GetLightRay(const Interaction& interaction) const
    {
        const float bias = 1e-3f;
        return Ray(interaction.m_HitPoint + bias * interaction.m_Normal, m_Direction);
    }

}   // namespace Hikari
