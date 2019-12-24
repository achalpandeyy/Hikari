#define _USE_MATH_DEFINES

#include "Sphere.h"
#include "Ray.h"

#include "glm/geometric.hpp"

#include <algorithm>
#include <cmath>

namespace Rays
{

inline bool solveQuadratic(float a, float b, float c, float& alpha, float& beta)
{
    float discriminant = b * b - 4.f * a * c;
    if (discriminant < 0.f)
        return false;
    else if (discriminant == 0.f)
        alpha = beta = -0.5 * (b / a);
    else
    {
        float q = b > 0.f ? -0.5f * (b + sqrt(discriminant)) : -0.5f * (b - sqrt(discriminant));
        alpha = q / a;
        beta = c / q;
    }
    return true;
}

bool Sphere::Intersect(const Ray& ray, float* distance) const
{
    float a = glm::dot(ray.m_Direction, ray.m_Direction);
    float b = 2.f * glm::dot(ray.m_Direction, ray.m_Origin - m_Center);
    float c = glm::dot(ray.m_Origin - m_Center, ray.m_Origin - m_Center) - (m_Radius * m_Radius);
    float t0, t1;

    if (!solveQuadratic(a, b, c, t0, t1))
        return false;

    if (t0 < 0.f && t1 < 0.f)
        return false;

    if (distance)
    {
        if (t0 > 0.f && t1 > 0.f)
            *distance = std::min(t0, t1);
        else if (t0 > 0.f)
            *distance = t0;
        else
            *distance = t1;
    }
    return true;
}

glm::vec3 Sphere::GetSurfaceNormal(const glm::vec3& hitPoint) const
{
    return glm::normalize(hitPoint - m_Center);
}

glm::vec2 Sphere::GetTextureCoordinates(const glm::vec3& hitPoint) const
{
    return glm::vec2
    (
        ((std::atan2(hitPoint.x, hitPoint.z) / M_PI) + 1.f) * 0.5f,
        std::acosf(hitPoint.y / m_Radius) / M_PI
    );
}

}   // namespace Rays