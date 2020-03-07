#pragma once

#include "Core/Shape.h"

namespace Hikari
{
    class TriangleMesh : public Shape
    {
    public:
        TriangleMesh(
            RTCDevice           device,
            RTCScene            scene,
            const Transform& objectToWorld,
            const char* path);

        ~TriangleMesh();

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

}	// namespace Hikari
