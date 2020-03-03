#include "Point.h"

#include "Core/Interaction.h"
#include "Math/Ray.h"

namespace Hikari
{
    glm::vec3 PointLight::Sample_Li(
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
        return m_Intensity / distance;
    }

    float PointLight::Pdf_Li(const Interaction& illumPoint, const glm::vec3& wi) const
    {
        // The probability of incoming light from any other direction except from the direction returned
        // by `Sample_Li` would be 0.
        //
        return 0.f;
    }

}   // namespace Hikari