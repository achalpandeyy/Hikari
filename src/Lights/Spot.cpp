#include "Spot.h"

#include "Core/Interaction.h"
#include "Math/Ray.h"

namespace Hikari
{
    glm::vec3 SpotLight::Sample_Li(
        const Interaction&  illumPoint,
        const glm::vec2&    e,
        glm::vec3*          wi,
        float*              pdf,
        VisibilityTester*   vis) const
    {
        *wi = glm::normalize(m_Position - illumPoint.m_Position);
        *pdf = 1.f;
        *vis = VisibilityTester(illumPoint, Interaction(m_Position));
        float distance = glm::dot(m_Position - illumPoint.m_Position, m_Position - illumPoint.m_Position);
        // if (distance < 1e-8f) distance = 1e-8f;
        return (m_Intensity * Falloff(-*wi)) / distance;
    }

    float SpotLight::Pdf_Li(const Interaction& illumPoint, const glm::vec3& wi) const
    {
        // The probability of incoming light from any other direction except from the direction returned
        // by `Sample_Li` would be 0.
        //
        return 0.f;
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