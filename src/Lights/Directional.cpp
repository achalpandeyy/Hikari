#include "Directional.h"

#include "Core/Interaction.h"
#include "Math/Ray.h"

namespace Hikari
{
    glm::vec3 DirectionalLight::Sample_Li(
        const Interaction&  illumPoint,
        const glm::vec2&    e,
        glm::vec3*          wi,
        float*              pdf,
        VisibilityTester*   vis) const
    {
        *wi = m_Direction;
        *pdf = 1.f;
        *vis = VisibilityTester(illumPoint, Interaction(glm::vec3(INFINITY)));
        return m_Radiance;
    }

    float DirectionalLight::Pdf_Li(const Interaction& illumPoint, const glm::vec3& wi) const
    {
        // The probability of incoming light from any other direction except from the direction returned
        // by `Sample_Li` would be 0.
        //
        return 0.f;
    };

}   // namespace Hikari

