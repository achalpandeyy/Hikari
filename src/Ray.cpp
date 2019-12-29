#include "Ray.h"

namespace Hikari
{

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction, float maxDistance)
    : m_Origin(origin), m_Direction(direction),
    m_ReciprocalDirection(1.f / direction), m_MaxDistance(maxDistance)
{
    for (unsigned int i = 0; i < 3; ++i)
    {
        m_DirectionSign[i] = m_ReciprocalDirection[i] < 0.f;
    }
}

}   // namespace Hikari