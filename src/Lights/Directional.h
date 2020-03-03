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
            : m_Direction(glm::normalize(direction)), m_Radiance(radiance)
        {}

        glm::vec3 GetIncidentRadiance(const glm::vec3& hitPoint) const override;
        Ray GetLightRay(const Interaction& interaction) const override;

    private:
        const glm::vec3 m_Direction;
        const glm::vec3 m_Radiance;
    };

}   // namespace Hikari

