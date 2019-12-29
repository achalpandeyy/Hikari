#pragma once

#include "glm/vec3.hpp"

namespace Hikari
{

class Light
{
public:
    virtual ~Light() {}

    enum E_LightType
    {
        E_POINT_LIGHT,
        E_DIRECTIONAL_LIGHT,
        E_SPOT_LIGHT
    } m_LightType;

    glm::vec3 m_Position;
    glm::vec3 m_Direction;

    // Returns the amount of incident light (product of intensity and color)
    // at the input hitPoint.
    virtual glm::vec3 GetIncidentLight(const glm::vec3& hitPoint) const = 0;

protected:
    Light(const glm::vec3&  color = glm::vec3(1.f), float intensity = 1.f)
        : m_Color(color), m_Intensity(intensity)
    {}

    glm::vec3 m_Color;
    float m_Intensity;
};

}   // namespace Rays