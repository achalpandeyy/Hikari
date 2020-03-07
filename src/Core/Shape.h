#pragma once

#include "Math/Transform.h"

#include <embree3/rtcore.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace Hikari
{
    class Shape
    {
    public:
        Shape(
            RTCGeometry geometry, const Transform& objectToWorld)
            : m_Geometry(geometry), m_ObjectToWorld(objectToWorld)
        {}

        virtual ~Shape() {}

        virtual float SurfaceArea() const = 0;

        virtual Interaction Sample(const Interaction& i, const glm::vec2& e, float* pdf) const = 0;

        virtual float Pdf(const Interaction& pShape, const glm::vec3& wi) const = 0;

        unsigned int m_Id;

    protected:
        RTCGeometry m_Geometry;
        Transform m_ObjectToWorld;
    };

}   // namespace Hikari
