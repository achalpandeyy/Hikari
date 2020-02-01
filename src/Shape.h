#pragma once

#include "Transform.h"

#include <embree3/rtcore.h>
#include <glm/glm.hpp>

#include <vector>

namespace Hikari
{
    class Transform;

    class Shape
    {
    public:
        Shape(
            RTCGeometry         geometry,
            const Transform&    objectToWorld,
            const glm::vec3&    albedo,
            const glm::vec3&    emission)
            : m_Geometry(geometry), m_ObjectToWorld(objectToWorld), m_Albedo(albedo),
            m_Emission(emission)
        {}

        virtual ~Shape() {}

        virtual void Release() = 0;
        virtual void Attach(RTCScene scene)
        {
            rtcAttachGeometry(scene, m_Geometry);
        }

        glm::vec3 m_Albedo;
        glm::vec3 m_Emission;

    protected:
        RTCGeometry m_Geometry;
        Transform m_ObjectToWorld;
    };

    class TriangleMesh : public Shape
    {
    public:
        TriangleMesh(
            RTCDevice           device,
            const Transform&    objectToWorld,
            const char*         path,
            const glm::vec3&    albedo,
            const glm::vec3&    emission);

        ~TriangleMesh();

        void Release() override;

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
            RTCDevice           device,
            const Transform&    objectToWorld,
            const glm::vec3&    albedo,
            const glm::vec3&    emission);

        ~Sphere();

        void Attach(RTCScene scene) override
        {
            m_ID = rtcAttachGeometry(scene, m_Geometry);
        }

        void Release() override;

        glm::vec3 m_Center;
        float m_Radius;
        unsigned int m_ID;

    private:
        static void Bounds(const RTCBoundsFunctionArguments* args);
        static void Intersect(const RTCIntersectFunctionNArguments* args);
        static void Occluded(const RTCOccludedFunctionNArguments* args);
    };

}   // namespace Hikari
