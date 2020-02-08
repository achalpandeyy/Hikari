#pragma once

#include "Light.h"

#include "glm/vec3.hpp"

namespace Hikari
{
    class PointLight : public Light
    {
    public:
        PointLight(const glm::vec3& position, const glm::vec3& intensity)
            : Light(intensity), m_Position(position)
        {}

        glm::vec3 GetIncidentLight(const glm::vec3& hitPoint) const override;
        Ray GetLightRay(const Interaction& interaction) const override;

    private:
        glm::vec3 m_Position;
    };

}   // namespace Hikari