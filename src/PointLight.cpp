#define _USE_MATH_DEFINES

#include "PointLight.h"

#include "glm/geometric.hpp"

#include <cmath>

namespace Hikari
{

glm::vec3 PointLight::GetIncidentLight(const glm::vec3& hitPoint) const
{
    float distanceToLightSquared = glm::dot(m_Position - hitPoint, m_Position - hitPoint);
    if (distanceToLightSquared < 1e-6)
    {
        distanceToLightSquared = 1e-6;
    }
    return (m_Intensity * m_Color) / glm::vec3(4.f * M_PI * distanceToLightSquared);
}

}   // namespace Hikari