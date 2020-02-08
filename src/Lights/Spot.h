#pragma once

#include "Light.h"

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
            : Light(intensity), m_Position(position), m_RefDirection(glm::normalize(lookAt - position)),
            m_CosTotalWidth(std::cos(glm::radians(totalWidth))), m_CosFalloffStart(std::cos(glm::radians(falloffStart)))
        {}
        
        glm::vec3 GetIncidentRadiance(const glm::vec3& hitPoint) const override;

        Ray GetLightRay(const Interaction& interaction) const override;

    private:
        float Falloff(const glm::vec3& direction) const;

        const glm::vec3 m_Position, m_RefDirection;
        const float m_CosTotalWidth, m_CosFalloffStart;
    };

}   // namespace Hikari
