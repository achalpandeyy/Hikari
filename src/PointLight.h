#pragma once

#include "Light.h"

#include "glm/vec3.hpp"

namespace Rays
{

class PointLight : public Light
{
public:
    PointLight(const glm::vec3& position, const glm::vec3& color = glm::vec3(1.f), float intensity = 1.f)
        : Light(color, intensity)
    {
        m_Position = position;
        m_LightType = E_POINT_LIGHT;
    }

    glm::vec3 GetIncidentLight(const glm::vec3& hitPoint) const;
};

}   // namespace Rays