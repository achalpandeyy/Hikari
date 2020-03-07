#pragma once

#include "Math/Ray.h"

#include <glm/glm.hpp>

namespace Hikari
{
    class Primitive;
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
            const Primitive*    primitive)
            : m_Position(position), m_Normal(normal), m_UV(uv), m_Primitive(primitive)
        {}

        Ray SpawnRayTo(const Interaction& i) const;

        glm::vec3 m_Position, m_Normal;
        glm::vec2 m_UV;

        // TODO(achal): Consider making it a shared_ptr.
        const Primitive* m_Primitive = nullptr;
    };

}   // namespace Hikari