#pragma once

#include "Core/Light.h"

#include <glm/glm.hpp>

#include <cmath>

namespace Hikari
{
    class Interaction;
    class Ray;

    class SpotLight : public Light
    {
    public:
        SpotLight(
            const glm::vec3&    position,
            const glm::vec3&    lookAt,
            const glm::vec3&    intensity,
            float               totalWidth,
            float               falloffStart)
            : Light(LightType::Spot), m_Position(position), m_RefDirection(glm::normalize(lookAt - position)),
            m_Intensity(intensity), m_CosTotalWidth(std::cos(glm::radians(totalWidth))),
            m_CosFalloffStart(std::cos(glm::radians(falloffStart)))
        {}

        glm::vec3 Sample_Li(
            const Interaction&  illumPoint,
            const glm::vec2&    e,
            glm::vec3*          wi,
            float*              pdf,
            VisibilityTester*   vis) const override;

        float Pdf_Li(const Interaction& illumPoint, const glm::vec3& wi) const override;

    private:
        float Falloff(const glm::vec3& direction) const;

        const glm::vec3 m_Position, m_RefDirection;
        const glm::vec3 m_Intensity;
        const float m_CosTotalWidth, m_CosFalloffStart;
    };

}   // namespace Hikari
