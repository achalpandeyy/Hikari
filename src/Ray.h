#pragma once

#include "glm/vec3.hpp"

namespace Hikari
{

class Ray
{
public:
    Ray(
        const glm::vec3&    origin,
        const glm::vec3&    direction,
        float               maxDistance = std::numeric_limits<float>::max());

    glm::vec3 m_Origin, m_Direction, m_ReciprocalDirection;
    unsigned int m_DirectionSign[3];

    // Max distance of a ray refers to the distance beyond which intersection
    // of the ray with an object won't be considered. This is useful for tackling
    // the false shadow bug - wherein an object which is beyond the light source
    // casts a shadown on the object (light source is in between the two objects).
    // Here we set the Max Distance to the distance to the light source so we don't
    // consider any intersections beyond that point.
    float m_MaxDistance;
};

}   // namespace Hikari