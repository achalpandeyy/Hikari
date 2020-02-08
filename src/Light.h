#pragma once

#include "glm/vec3.hpp"

namespace Hikari
{
    class Interaction;
    class Ray;

    class Light
    {
    public:
        virtual ~Light() {}

        // Returns the amount of incident radiance (W / sr * m^2) at the input hitPoint.
        //
        virtual glm::vec3 GetIncidentRadiance(const glm::vec3& hitPoint) const = 0;

        virtual Ray GetLightRay(const Interaction& interaction) const = 0;

    protected:
        Light(const glm::vec3& intensity) : m_Intensity(intensity) {}

        // Radiant Intensity - Amount of power emitted from the light source per unit solid angle (W / sr).
        //
        glm::vec3 m_Intensity;
    };

}   // namespace Hikari