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
            const glm::vec2&    uv,
            const Shape*        shape)
            : m_HitPoint(hitPoint), m_Normal(normal), m_UV(uv), m_Shape(shape)
        {}

        glm::vec3 m_HitPoint, m_Normal;
        glm::vec2 m_UV;

        // TODO(achal): Make it a shared_ptr.
        const Shape* m_Shape = nullptr;
    };

}   // namespace Hikari