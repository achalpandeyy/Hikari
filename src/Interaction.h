#pragma once

#include <glm/glm.hpp>

namespace Hikari
{
    class Shape;

    class Interaction
    {
    public:
        Interaction(
            const glm::vec3&    hitPoint,
            const glm::vec3&    normal,
            const Shape*        shape)
            : m_HitPoint(hitPoint), m_Normal(normal), m_Shape(shape)
        {}

        glm::vec3 m_HitPoint, m_Normal;
        const Shape* m_Shape;
    };

}   // namespace Hikari