#pragma once

#include "Light.h"

#include <glm/glm.hpp>

namespace Hikari
{
    class PointLight : public Light
    {
    public:
        PointLight(const glm::vec3& position, const glm::vec3& intensity)
            : Light(intensity), m_Position(position)
        {}

        glm::vec3 GetIncidentRadiance(const glm::vec3& hitPoint) const override;
        Ray GetLightRay(const Interaction& interaction) const override;

    private:
        glm::vec3 m_Position;
    };

}   // namespace Hikari