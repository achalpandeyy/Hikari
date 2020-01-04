#pragma once

#include "glm/geometric.hpp"
#include "glm/vec3.hpp"

namespace Hikari
{

class Ray
{
public:
    Ray(
        const glm::vec3&    origin,
        const glm::vec3&    direction,
        float               tMax = std::numeric_limits<float>::max())
        : m_Origin(origin), m_Direction(direction),
        m_tMax(tMax)
    {}

    inline glm::vec3 operator () (float t) const
    {
        return m_Origin + t * m_Direction;
    }

    glm::vec3 m_Origin, m_Direction;

    // Max distance of a ray refers to the distance beyond which intersection
    // of the ray with an object won't be considered. This is useful for tackling
    // the false shadow bug - wherein an object which is beyond the light source
    // casts a shadown on the object (light source is in between the two objects).
    // Here we set the Max Distance to the distance to the light source so we don't
    // consider any intersections beyond that point.
    float m_tMax;
};

}   // namespace Hikari