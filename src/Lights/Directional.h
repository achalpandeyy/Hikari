#pragma once

#include "Core/Light.h"

#include <glm/glm.hpp>

namespace Hikari
{
    class Interaction;
    class Ray;

    class DirectionalLight : public Light
    {
    public:
        DirectionalLight(const glm::vec3& direction, const glm::vec3& radiance)
            : Light(LightType::Directional), m_Direction(glm::normalize(direction)), m_Radiance(radiance)
        {}

        glm::vec3 Sample_Li(
            const Interaction&  illumPoint,
            const glm::vec2&    e,
            glm::vec3*          wi,
            float*              pdf,
            VisibilityTester*   vis) const override;

        float Pdf_Li(const Interaction& illumPoint, const glm::vec3& wi) const override;

    private:
        const glm::vec3 m_Direction, m_Radiance;
    };

}   // namespace Hikari

