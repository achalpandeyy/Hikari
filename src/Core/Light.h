#pragma once

#include "Core/Interaction.h"

#include <glm/glm.hpp>

namespace Hikari
{
    class Ray;
    class Scene;
    class VisibilityTester;

    enum class LightType
    {
        Point,
        Directional,
        Spot,
        Area
    };

    class Light
    {
    public:
        Light(LightType type, unsigned int numSamples = 1u) : m_Type(type), m_NumSamples(numSamples) {}

        virtual ~Light() {}

        virtual glm::vec3 Sample_Li(
            const Interaction&  illumPoint,
            const glm::vec2&    e,
            glm::vec3*          wi,
            float*              pdf,
            VisibilityTester*   vis) const = 0;

        virtual float Pdf_Li(const Interaction& illumPoint, const glm::vec3& wi) const = 0;

        const LightType m_Type;
        const unsigned int m_NumSamples;
    };

    class VisibilityTester
    {
    public:
        VisibilityTester() = default;

        VisibilityTester(const Interaction& p0, const Interaction& p1) : m_P0(p0), m_P1(p1) {}

        bool Unoccluded(const Scene& scene) const;

        Interaction m_P0, m_P1;
    };

}   // namespace Hikari