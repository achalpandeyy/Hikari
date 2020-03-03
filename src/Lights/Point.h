#pragma once

#include "Core/Light.h"

#include <glm/glm.hpp>

namespace Hikari
{
    class PointLight : public Light
    {
    public:
        PointLight(const glm::vec3& position, const glm::vec3& intensity)
            : Light(LightType::Point), m_Position(position), m_Intensity(intensity)
        {}

        glm::vec3 Sample_Li(
            const Interaction&  illumPoint,
            const glm::vec2&    e,
            glm::vec3*          wi,
            float*              pdf,
            VisibilityTester*   vis) const override;

        float Pdf_Li(const Interaction& illumPoint, const glm::vec3& wi) const override;

    private:
        const glm::vec3 m_Position, m_Intensity;
    };

}   // namespace Hikari