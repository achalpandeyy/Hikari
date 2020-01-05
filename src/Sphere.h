#pragma once

#define _USE_MATH_DEFINES

#include "Shape.h"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/common.hpp"
#include "glm/trigonometric.hpp"

#include <algorithm>
#include <math.h>

namespace Hikari
{

class Interaction;
class Ray;
class Transform;

class Sphere : public Shape
{
public:
    Sphere(
        const Transform*    objectToWorld,
        const Transform*    worldToObject,
        bool                reverseOrientation,
        float               radius,
        float               yMin,
        float               yMax,
        float               phiMax = 2.f * M_PI,
        const glm::vec3& 	albedo = glm::vec3(0.18f))
        : Shape(objectToWorld, worldToObject, reverseOrientation),
        m_Radius(radius), m_YMin(glm::clamp(std::min(yMin, yMax), -radius, radius)),
        m_YMax(glm::clamp(std::max(yMin, yMax), -radius, radius)),
        m_ThetaMin(std::acos(glm::clamp(m_YMin / radius, -1.f, 1.f))),
        m_ThetaMax(std::acos(glm::clamp(m_YMax / radius, -1.f, 1.f))),
        m_PhiMax(glm::radians(glm::clamp(phiMax, 0.f, 360.f))),
        m_Albedo(albedo)
    {}

    bool Intersect(const Ray& ray, float* t, Interaction* interaction) const;

private:
    const float m_Radius;
    const float m_YMin, m_YMax;
    const float m_ThetaMin, m_ThetaMax, m_PhiMax;
    glm::vec3 m_Albedo;
};

}   // namespace Hikari