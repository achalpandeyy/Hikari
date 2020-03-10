#pragma once

#include "Core/Shape.h"

namespace Hikari
{
    class Sphere : public Shape
    {
    public:
        Sphere(
            const glm::vec3&    center,
            float               radius,
            RTCDevice           device,
            RTCScene            scene);

        ~Sphere();

        float SurfaceArea() const override;

        Interaction Sample(const Interaction& i, const glm::vec2& e, float* pdf) const override;

        float Pdf(const Interaction& pShape, const glm::vec3& wi) const override;

    private:
        static void Bounds(const RTCBoundsFunctionArguments* args);
        static void Intersect(const RTCIntersectFunctionNArguments* args);
        static void Occluded(const RTCOccludedFunctionNArguments* args);

        glm::vec3 m_Center;
        float m_Radius;
    };

}	// namespace Hikari