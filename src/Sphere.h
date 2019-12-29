#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

namespace Hikari
{

class Ray;

class Sphere
{
public:
    float m_Radius;
    glm::vec3 m_Center;
    glm::vec3 m_Albedo;

    Sphere(
        float               radius,
        const glm::vec3& 	center,
        const glm::vec3& 	albedo = glm::vec3(0.18f))
        : m_Radius(radius), m_Center(center), m_Albedo(albedo)
    {}

    bool Intersect(const Ray& ray, float* distance) const;
    glm::vec3 GetSurfaceNormal(const glm::vec3& hitPoint) const;
    glm::vec2 GetTextureCoordinates(const glm::vec3& hitPoint) const;
};

}   // namespace Hikari