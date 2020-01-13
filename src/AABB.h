#pragma once

#include "Ray.h"

#include "glm/vec3.hpp"
#include "glm/common.hpp"

namespace Hikari
{

    class AABB
    {
    public:
        AABB()
            : m_PointMin(glm::vec3(std::numeric_limits<float>::max())),
            m_PointMax(glm::vec3(std::numeric_limits<float>::lowest()))
        {}

        AABB(const glm::vec3& point)
            : m_PointMin(point), m_PointMax(point)
        {}

        AABB(const glm::vec3& p1, const glm::vec3& p2)
            : m_PointMin(glm::min(p1, p2)), m_PointMax(glm::max(p1, p2))
        {}

        const glm::vec3& operator [] (int i) const;
        glm::vec3& operator [] (int i);
        glm::vec3 Corner(int corner) const;
        glm::vec3 Diagonal() const;
        float SurfaceArea() const;
        float Volume() const;
        int MaxExtent() const;

        inline bool Intersect(const Ray& ray, float* t0, float* t1) const
        {
            float hitt0 = 0.f, hitt1 = ray.m_tMax;
            for (unsigned int i = 0; i < 3; ++i)
            {
                float invRayDirection = 1.f / ray.m_Direction[i];
                float tNear = (m_PointMin[i] - ray.m_Origin[i]) * invRayDirection;
                float tFar = (m_PointMax[i] - ray.m_Origin[i]) * invRayDirection;

                if (tNear > tFar) std::swap(tNear, tFar);

                hitt0 = tNear > hitt0 ? tNear : hitt0;
                hitt1 = tFar < hitt1 ? tFar : hitt1;

                if (hitt0 > hitt1) return false;
            }
            if (t0) *t0 = hitt0;
            if (t1) *t1 = hitt1;
            return true;
        }

        glm::vec3 m_PointMin;
        glm::vec3 m_PointMax;
    };

    AABB Union(const AABB& box, const glm::vec3& point);
    AABB Union(const AABB& box1, const AABB& box2);
    AABB Intersection(const AABB& box1, const AABB& box2);
    bool Overlaps(const AABB& box1, const AABB& box2);
    bool Inside(const glm::vec3& point, const AABB& box);

    // Doesn't consider points on the upper (max) boundary of the
    // box to be inside the box
    bool InsideExclusive(const glm::vec3& point, const AABB& box);

    AABB Expand(const AABB& box, float delta);

}   // namespace Hikari
