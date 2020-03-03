#pragma once

#include "Math/Ray.h"

#include <glm/glm.hpp>

namespace Hikari
{
    class Shape;

    class Interaction
    {
    public:
        Interaction() = default;

        Interaction(const glm::vec3& position) : m_Position(position) {}

        Interaction(
            const glm::vec3&    position,
            const glm::vec3&    normal,
            const glm::vec2&    uv,
            const Shape*        shape)
            : m_Position(position), m_Normal(normal), m_UV(uv), m_Shape(shape)
        {}

        Ray SpawnRayTo(const Interaction& i) const;

        glm::vec3 m_Position, m_Normal;
        glm::vec2 m_UV;

        // TODO(achal): Make it a shared_ptr.
        const Shape* m_Shape = nullptr;
    };

}   // namespace Hikari