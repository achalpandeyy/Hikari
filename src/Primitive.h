#pragma once

#include "Shape.h"

#include "glm/vec3.hpp"

#include <iostream>
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

        bool Intersect(const Ray& ray, Interaction& interaction) const override;
        bool IntersectP(const Ray& ray) const override;

        glm::vec3 GetAlbedo() const override { return m_Albedo; }

    private:
        std::shared_ptr<Shape> m_Shape;
        glm::vec3 m_Albedo;
    };

    class Aggregate : public Primitive
    {
        // This method should never be called. `Interaction` should hold
        // the pointer to `Primitive` which the ray actually hits, not the
        // whole `Aggregate` because it is used to obtain material properties
        // of the object.
        virtual glm::vec3 GetAlbedo() const override
        {
            std::cerr <<
                "Aggregate::GetAlbedo() method called"
                "; should have gone to GeometricPrimitive" << std::endl;
            return glm::vec3(0.f);
        }
    };

}   // namespace Hikari