#pragma once

#include <embree3/rtcore.h>
#include <glm/glm.hpp>

#include <vector>

namespace Hikari
{
    class Transform;

    class EmbreeGeometry
    {
    public:
        EmbreeGeometry(RTCGeometry geometry, const glm::vec3& albedo)
            : m_Geometry(geometry), m_Albedo(albedo)
        {}

        virtual ~EmbreeGeometry() {}

        virtual void Release() = 0;
        unsigned int Attach(RTCScene scene) { return rtcAttachGeometry(scene, m_Geometry); }

        unsigned int GetId() const { return m_Id; }
        void SetId(unsigned int id) { m_Id = id; }

        glm::vec3 GetAlbedo() const { return m_Albedo; }

    protected:
        RTCGeometry m_Geometry;
        unsigned int m_Id;

    private:
        glm::vec3 m_Albedo;
    };

    class EmbreeTriangleMesh : public EmbreeGeometry
    {
    public:
        EmbreeTriangleMesh(
            RTCDevice           device,
            const char*         path,
            const Transform&    objectToWorld,
            const glm::vec3&    albedo);

        ~EmbreeTriangleMesh();

        void Release() override;

    private:
        std::vector<glm::vec3> m_VertexPositions, m_VertexNormals;
        std::vector<glm::vec2> m_VertexUV;
        std::vector<glm::uvec3> m_Indices;
        RTCBuffer m_VertexPositionBuffer = nullptr, m_IndexBuffer = nullptr;
    };

    class EmbreeSphere : public EmbreeGeometry
    {
    public:
        EmbreeSphere(
            RTCDevice           device,
            const glm::vec3&    center,
            float               radius,
            const glm::vec3&    albedo);

        ~EmbreeSphere();

        void Release() override;

        glm::vec3 m_Center;
        float m_Radius;
    };

}   // namespace Hikari
