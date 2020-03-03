#pragma once

#include "Material.h"
#include "Math/Transform.h"

#include <embree3/rtcore.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace Hikari
{
    class Transform;

    class Shape
    {
    public:
        Shape(
            RTCGeometry                         geometry,
            const Transform&                    objectToWorld,
            const std::shared_ptr<Material>&    material,
            const glm::vec3&                    emission)
            : m_Geometry(geometry), m_ObjectToWorld(objectToWorld), m_Material(material),
            m_Emission(emission)
        {}

        virtual ~Shape() {}

        virtual void Release() = 0;
        virtual void Attach(RTCScene scene)
        {
            rtcAttachGeometry(scene, m_Geometry);
        }

        virtual float SurfaceArea() const = 0;

        virtual Interaction Sample(const Interaction& i, const glm::vec2& e, float* pdf) const = 0;

        virtual float Pdf(const Interaction& pShape, const glm::vec3& wi) const = 0;

        glm::vec3 m_Emission;
        std::shared_ptr<Material> m_Material;

    protected:
        RTCGeometry m_Geometry;
        Transform m_ObjectToWorld;
    };

    class TriangleMesh : public Shape
    {
    public:
        TriangleMesh(
            RTCDevice                           device,
            const Transform&                    objectToWorld,
            const char*                         path,
            const std::shared_ptr<Material>&    material,
            const glm::vec3&                    emission);

        ~TriangleMesh();

        void Release() override;

        ///////////////////////////////////////////////////////////////////////
        // TODO(achal): Implement the following methods for triangle meshes. //
        ///////////////////////////////////////////////////////////////////////

        float SurfaceArea() const override
        {
            return 1.f;
        }

        Interaction Sample(const Interaction& i, const glm::vec2& e, float* pdf) const override
        {
            return Interaction();
        }

        float Pdf(const Interaction& pShape, const glm::vec3& wi) const override
        {
            return 1.f;
        }

    private:
        std::vector<glm::vec3> m_VertexPositions, m_VertexNormals;
        std::vector<glm::vec2> m_VertexUV;
        std::vector<glm::uvec3> m_Indices;
        RTCBuffer m_VertexPositionBuffer = nullptr, m_IndexBuffer = nullptr;
    };

    class Sphere : public Shape
    {
    public:
        Sphere(
            RTCDevice                           device,
            const Transform&                    objectToWorld,
            const std::shared_ptr<Material>&    material,
            const glm::vec3&                    emission);

        ~Sphere();

        void Attach(RTCScene scene) override
        {
            m_ID = rtcAttachGeometry(scene, m_Geometry);
        }

        void Release() override;

        float SurfaceArea() const override;

        Interaction Sample(const Interaction& i, const glm::vec2& e, float* pdf) const override;

        float Pdf(const Interaction& pShape, const glm::vec3& wi) const override;

        glm::vec3 m_Center;
        float m_Radius;
        unsigned int m_ID;

    private:
        static void Bounds(const RTCBoundsFunctionArguments* args);
        static void Intersect(const RTCIntersectFunctionNArguments* args);
        static void Occluded(const RTCOccludedFunctionNArguments* args);
    };

}   // namespace Hikari
