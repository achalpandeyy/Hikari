#pragma once

#include "Light.h"

#include "glm/vec3.hpp"
#include "glm/geometric.hpp"

namespace Rays
{

class DirectionalLight : public Light
{
public:
    DirectionalLight(const glm::vec3& direction, const glm::vec3& color = glm::vec3(1.f), float intensity = 1.f)
        : Light(color, intensity)
    {
        m_Direction = glm::normalize(direction);
        m_LightType = Light::E_DIRECTIONAL_LIGHT;
    }

    glm::vec3 GetIncidentLight(const glm::vec3& hitPoint) const;
};

}   // namespace Rays

