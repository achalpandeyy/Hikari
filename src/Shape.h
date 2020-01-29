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
            const glm::vec3&    albedo)
            : m_Geometry(geometry), m_ObjectToWorld(objectToWorld), m_Albedo(albedo)
        {}

        virtual ~Shape() {}

        virtual void Release() = 0;
        unsigned int Attach(RTCScene scene)
        {
            return rtcAttachGeometry(scene, m_Geometry);
        }

        unsigned int GetId() const { return m_Id; }
        void SetId(unsigned int id) { m_Id = id; }

        glm::vec3 GetAlbedo() const { return m_Albedo; }

    protected:
        RTCGeometry m_Geometry;
        Transform m_ObjectToWorld;
        unsigned int m_Id;

    private:
        glm::vec3 m_Albedo;
    };

    class TriangleMesh : public Shape
    {
    public:
        TriangleMesh(
            RTCDevice           device,
            const Transform&    objectToWorld,
            const char*         path,
            const glm::vec3&    albedo);

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
            const glm::vec3&    albedo);

        ~Sphere();

        void Release() override;

        glm::vec3 m_Center;
        float m_Radius;
    };

}   // namespace Hikari
