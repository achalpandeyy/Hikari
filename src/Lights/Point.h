#pragma once

#include "Light.h"

#include <glm/glm.hpp>

namespace Hikari
{
    class PointLight : public Light
    {
    public:
        PointLight(const glm::vec3& position, const glm::vec3& intensity)
            : m_Position(position), m_Intensity(intensity)
        {}

        glm::vec3 GetIncidentRadiance(const glm::vec3& hitPoint) const override;
        Ray GetLightRay(const Interaction& interaction) const override;

    private:
        const glm::vec3 m_Position;

        // Radiant Intensity
        // Amount of power emitted from the light source per unit solid angle (W / sr)
        //
        const glm::vec3 m_Intensity;
    };

}   // namespace Hikari