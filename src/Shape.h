#pragma once

#include "Transform.h"

namespace Hikari
{

class Shape
{
public:
    Shape(
        const Transform*    objectToWorld,
        const Transform*    worldToObject,
        const glm::vec3&    albedo,
        bool                reverseOrientation)
        : m_ObjectToWorld(objectToWorld), m_WorldToObject(worldToObject), m_Albedo(albedo),
        m_ReverseOrientation(reverseOrientation),
        m_TransformSwapsHandedness(m_ObjectToWorld->SwapsHandedness())
    {}
    virtual ~Shape() {}

    virtual bool Intersect(const Ray& ray, float* t, Interaction* interaction) const = 0;
    virtual bool IntersectP(const Ray& ray) const
    {
        float t = ray.m_tMax;
        Interaction interaction;
        return Intersect(ray, &t, &interaction);
    }

    const Transform* m_ObjectToWorld;
    const Transform* m_WorldToObject;
    const glm::vec3 m_Albedo;
    const bool m_ReverseOrientation;
    const bool m_TransformSwapsHandedness;
};

}   // namespace Hikari