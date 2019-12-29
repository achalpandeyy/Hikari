#include "DirectionalLight.h"

namespace Hikari
{

glm::vec3 DirectionalLight::GetIncidentLight(const glm::vec3& hitPoint) const
{
    return m_Intensity * m_Color;
}

}   // namespace Hikari

