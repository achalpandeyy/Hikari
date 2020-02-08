#pragma once

#include "Light.h"

#include <glm/glm.hpp>

namespace Hikari
{
    class Interaction;
    class Ray;

    class DirectionalLight : public Light
    {
    public:
        DirectionalLight(const glm::vec3& direction, const glm::vec3& intensity)
            : Light(intensity), m_Direction(glm::normalize(direction))
        {}

        glm::vec3 GetIncidentLight(const glm::vec3& hitPoint) const override;
        Ray GetLightRay(const Interaction& interaction) const override;

    private:
        glm::vec3 m_Direction;
    };

}   // namespace Hikari

