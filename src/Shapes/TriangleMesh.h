#pragma once

#include "Core/Shape.h"

#include <vector>

namespace Hikari
{
    class Transform;

    class TriangleMesh : public Shape
    {
    public:
        TriangleMesh(
            const char*         path,
            const Transform&    objectToWorld,
            RTCDevice           device,
            RTCScene            scene);

        ~TriangleMesh();

        ///////////////////////////////////////////////////////////////////////
        // TODO(achal): Implement the following methods for triangle meshes. //
        ///////////////////////////////////////////////////////////////////////

        float SurfaceArea() const override
        {
            return 1.f;
        }

        Interaction AreaSample(const glm::vec2& sample, float* pdf) const override
        {
            return Interaction();
        }

        float Pdf(const Interaction& pShape, const glm::vec3& wi) const override
        {
            return 1.f;
        }

        Interaction SolidAngleSample(const Interaction& illumPoint, const glm::vec2& sample, float* pdf) const override
        {
            return Interaction();
        }

    private:
        std::vector<glm::vec3> m_VertexPositions, m_VertexNormals;
        std::vector<glm::vec2> m_VertexUV;
        std::vector<glm::uvec3> m_Indices;
        RTCBuffer m_VertexPositionBuffer = nullptr, m_IndexBuffer = nullptr;
    };

    std::shared_ptr<TriangleMesh> CreateTriangleMeshShape(
        const char*         path,
        const glm::vec3&    translate,
        float               degrees,
        const glm::vec3&    rotationAxis,
        const glm::vec3&    scale,
        RTCDevice           rtcDevice,
        RTCScene            rtcScene);

}	// namespace Hikari
