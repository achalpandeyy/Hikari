#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

namespace Hikari
{

    class Interaction
    {
    public:
        Interaction() {}
        Interaction(
            const glm::vec3&    hitPoint,
            const glm::vec3&    normal)
            : m_HitPoint(hitPoint), m_Normal(normal)
        {}

        // Temporary member variable to store if the `this` Interaction is valid
        // which the RTEngine tells us.
        bool m_Valid;

        glm::vec3 m_HitPoint, m_Normal, m_Albedo;
    };

}   // namespace Hikari