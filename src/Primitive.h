#pragma once

#include "Shape.h"

#include "glm/vec3.hpp"

#include <memory>

namespace Hikari
{

class Interaction;
class Ray;

class Primitive
{
public:
    virtual ~Primitive() {}

    virtual bool Intersect(const Ray& ray, Interaction& interaction) const = 0;
    virtual bool IntersectP(const Ray& ray) const = 0;

    virtual glm::vec3 GetAlbedo() const = 0;
};

class GeometricPrimitive : public Primitive
{
public:
    GeometricPrimitive(const std::shared_ptr<Shape>& shape, const glm::vec3& albedo)
        : m_Shape(shape), m_Albedo(albedo)
    {}

    bool Intersect(const Ray& ray, Interaction& interaction) const;
    bool IntersectP(const Ray& ray) const;

    glm::vec3 GetAlbedo() const { return m_Albedo; }

private:
    std::shared_ptr<Shape> m_Shape;
    glm::vec3 m_Albedo;
};

}   // namespace Hikari