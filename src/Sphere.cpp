#include "Sphere.h"

#include "Interaction.h"
#include "Ray.h"
#include "Transform.h"

#include "glm/geometric.hpp"

#include <algorithm>

namespace Hikari
{

inline bool Quadratic(float a, float b, float c, float* t0, float* t1)
{
    float discriminant = b * b - 4.f * a * c;
    if (discriminant < 0.f)
        return false;
    else if (discriminant == 0.f)
        *t0 = *t1 = -0.5 * (b / a);
    else
    {
        float rootDiscriminant = sqrt(discriminant);
        float q = b > 0.f ? -0.5f * (b + rootDiscriminant) : -0.5f * (b - rootDiscriminant);
        *t0 = q / a;
        *t1 = c / q;

        if (*t0 > *t1) std::swap(*t0, *t1);
    }
    return true;
}

bool Sphere::Intersect(const Ray& r, float* t, Interaction* interaction) const
{
    float phi;
    glm::vec3 pHit;

    Ray ray = (*m_WorldToObject)(r);

    float a = glm::dot(ray.m_Direction, ray.m_Direction);
    float b = 2.f * glm::dot(ray.m_Direction, ray.m_Origin);
    float c = glm::dot(ray.m_Origin, ray.m_Origin) - (m_Radius * m_Radius);

    float t0, t1;
    if (!Quadratic(a, b, c, &t0, &t1))
    {
        return false;
    }

    // This works because, `Quadratic` gurantees that `t0` is always less than `t1`
    if (t0 > ray.m_tMax || t1 <= 0.f)
    {
        return false;
    }

    float tShapeHit = t0;
    if (tShapeHit <= 0.f)
    {
        tShapeHit = t1;
        if (tShapeHit > ray.m_tMax)
        {
            return false;
        }
    }

    pHit = ray(tShapeHit);
    phi = std::atan2(pHit.z, pHit.x);

    // `std::atan2` returns values in the range [-Pi, +Pi] so convert it to
    // the range [0, 2 * PI)
    if (phi < 0.f) phi += 2.f * M_PI;

    // Test against clipping params.
    // To test against "y" clipping params it is important to first check
    // if the values `m_YMin` or `m_YMax` actually clips the sphere - which
    // is done with the `m_YMin > -m_Radius` line, otherwise some intersections
    // which lie just above the surface of the sphere (due to floating-point error)
    // may be misinterpreted.
    if ((m_YMin > -m_Radius && pHit.y < m_YMin) || (m_YMax < m_Radius && pHit.y > m_YMax)
        || phi > m_PhiMax)
    {
        if (tShapeHit == t1) return false;
        if (t1 > ray.m_tMax) return false;

        tShapeHit = t1;

        // Compute hitpoint and phi again for the case when `tShapeHit` = `t1`
        pHit = ray(tShapeHit);
        phi = std::atan2(pHit.y, pHit.x);
        if (phi < 0.f) phi += 2.f * M_PI;

        if ((m_YMin > -m_Radius && pHit.y < m_YMin) ||
            (m_YMax < m_Radius && pHit.y > m_YMax) ||
            phi > m_PhiMax)
            return false;
    }

    float u = phi / m_PhiMax;
    float theta = std::acos(glm::clamp(pHit.y / m_Radius, -1.f, 1.f));
    float v = (theta - m_ThetaMin) / (m_ThetaMax - m_ThetaMin);

    // Compute `dpdu` and `dpdv`
    float yRadius = std::sqrt(pHit.x * pHit.x + pHit.z * pHit.z);
    float invYRadius = 1.f / yRadius;
    float cosPhi = pHit.x * invYRadius;
    float sinPhi = pHit.z * invYRadius;
    glm::vec3 dpdu(-m_PhiMax * pHit.z, 0.f, m_PhiMax * pHit.x);
    glm::vec3 dpdv =
        (m_ThetaMax - m_ThetaMin) * glm::vec3(pHit.y * cosPhi, -m_Radius * std::sin(theta), pHit.y * sinPhi);

    // Compute auxiliary things for calculating coefficients for fundamental forms
    glm::vec3 d2pduu = -m_PhiMax * m_PhiMax * glm::vec3(pHit.x, 0.f, pHit.z);
    glm::vec3 d2pdudv = m_PhiMax * pHit.y * (m_ThetaMax - m_ThetaMin) * glm::vec3(-sinPhi, 0.f, cosPhi);
    glm::vec3 d2pdvv = -(m_ThetaMax - m_ThetaMin) * (m_ThetaMax - m_ThetaMin) * glm::vec3(pHit.x, pHit.y, pHit.z);

    // Compute coefficients for fundamental forms
    float E = glm::dot(dpdu, dpdu);
    float F = glm::dot(dpdu, dpdv);
    float G = glm::dot(dpdv, dpdv);
    glm::vec3 normal = glm::normalize(glm::cross(dpdv, dpdu));
    float e = glm::dot(normal, d2pduu);
    float f = glm::dot(normal, d2pdudv);
    float g = glm::dot(normal, d2pdvv);

    // Compute `dndu` and `dndv`
    float invEGF2 = 1.f / (E * G - F * F);
    glm::vec3 dndu = glm::vec3((f * F - e * G) * invEGF2 * dpdu +
        (e * F - f * E) * invEGF2 * dpdv);
    glm::vec3 dndv = glm::vec3((g * F - f * G) * invEGF2 * dpdu +
        (f * F - g * E) * invEGF2 * dpdv);

    *interaction = (*m_ObjectToWorld)(Interaction(pHit, glm::vec2(u, v), dpdu, dpdv, dndu, dndv, m_Albedo, this));
    *t = tShapeHit;
    return true;   
}

bool Sphere::IntersectP(const Ray& r) const
{
    float phi;
    glm::vec3 pHit;

    Ray ray = (*m_WorldToObject)(r);

    float a = glm::dot(ray.m_Direction, ray.m_Direction);
    float b = 2.f * glm::dot(ray.m_Direction, ray.m_Origin);
    float c = glm::dot(ray.m_Origin, ray.m_Origin) - (m_Radius * m_Radius);

    float t0, t1;
    if (!Quadratic(a, b, c, &t0, &t1))
    {
        return false;
    }

    if (t0 > ray.m_tMax || t1 <= 0.f)
    {
        return false;
    }

    float tShapeHit = t0;
    if (tShapeHit <= 0.f)
    {
        tShapeHit = t1;
        if (tShapeHit > ray.m_tMax)
        {
            return false;
        }
    }

    pHit = ray(tShapeHit);
    phi = std::atan2(pHit.z, pHit.x);

    if (phi < 0.f) phi += 2.f * M_PI;

    if ((m_YMin > -m_Radius && pHit.y < m_YMin) || (m_YMax < m_Radius && pHit.y > m_YMax)
        || phi > m_PhiMax)
    {
        if (tShapeHit == t1) return false;
        if (t1 > ray.m_tMax) return false;

        tShapeHit = t1;

        // Compute hitpoint and phi again for the case when `tShapeHit` = `t1`
        pHit = ray(tShapeHit);
        phi = std::atan2(pHit.y, pHit.x);
        if (phi < 0.f) phi += 2.f * M_PI;

        if ((m_YMin > -m_Radius && pHit.y < m_YMin) ||
            (m_YMax < m_Radius && pHit.y > m_YMax) ||
            phi > m_PhiMax)
            return false;
    }
    return true;
}

float Sphere::Area() const
{
    return m_PhiMax * m_Radius * (m_YMax - m_YMin);
}

}   // namespace Hikari