#pragma once

#include "Core/Interaction.h"

#include <embree3/rtcore.h>
#include <glm/glm.hpp>

namespace Hikari
{
    class Shape
    {
    public:
        Shape(RTCGeometry geometry) : m_Geometry(geometry) {}

        virtual ~Shape() {}

        virtual float SurfaceArea() const = 0;

        virtual Interaction AreaSample(const glm::vec2& sample, float* pdf) const = 0;

        virtual float Pdf(const Interaction& pShape, const glm::vec3& wi) const = 0;

        virtual Interaction SolidAngleSample(const Interaction& illumPoint, const glm::vec2& sample, float* pdf) const = 0;

        unsigned int m_Id;

    protected:
        RTCGeometry m_Geometry;
    };

}   // namespace Hikari
