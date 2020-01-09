#include "Interaction.h"

#include "Shape.h"

namespace Hikari
{

Interaction::Interaction(
    const glm::vec3&    hitPoint,
    const glm::vec3&    normal,
    const glm::vec3&    albedo,
    const Shape*        shape)
    : m_HitPoint(hitPoint), m_Normal(normal), m_Albedo(albedo), m_Shape(shape)
{
    if (m_Shape && (m_Shape->m_ReverseOrientation ^ m_Shape->m_TransformSwapsHandedness))
    {
        m_Normal *= -1.f;
    }
}

}   // namespace Hikari