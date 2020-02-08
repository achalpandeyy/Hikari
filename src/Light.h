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

        // Returns the amount of incident light (product of intensity and color)
        // at the input hitPoint.
        //
        virtual glm::vec3 GetIncidentLight(const glm::vec3& hitPoint) const = 0;

        virtual Ray GetLightRay(const Interaction& interaction) const = 0;

    protected:
        Light(const glm::vec3& intensity) : m_Intensity(intensity) {}

        // Amount of power emitted from the light source per unit solid angle (W/sr).
        //
        glm::vec3 m_Intensity;
    };

}   // namespace Hikari