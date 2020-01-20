#include "EmbreeGeometry.h"

#include "ObjLoader.h"

#include <iostream>

namespace Hikari
{

    EmbreeTriangleMesh::EmbreeTriangleMesh(
        RTCDevice           device,
        const char*         path,
        const Transform&    objectToWorld,
        const glm::vec3&    albedo)
        : EmbreeGeometry(rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE), albedo)
    {
        // Load the obj file.
        if (LoadObj(path, m_VertexPositions, m_VertexNormals, m_VertexUV, m_Indices,
            objectToWorld))
        {
            m_VertexPositionBuffer = rtcNewSharedBuffer(device, m_VertexPositions.data(),
                m_VertexPositions.size() * sizeof(glm::vec3));
            m_IndexBuffer = rtcNewSharedBuffer(device, m_Indices.data(),
                m_Indices.size() * sizeof(glm::uvec3));

            rtcSetGeometryBuffer(m_Geometry, RTC_BUFFER_TYPE_VERTEX, 0u, RTC_FORMAT_FLOAT3,
                m_VertexPositionBuffer, 0u, sizeof(glm::vec3), m_VertexPositions.size());
            rtcSetGeometryBuffer(m_Geometry, RTC_BUFFER_TYPE_INDEX, 0u, RTC_FORMAT_UINT3,
                m_IndexBuffer, 0, sizeof(glm::uvec3), m_Indices.size());

            rtcCommitGeometry(m_Geometry);
        }
        else
        {
            std::cerr << "Failed to load model at path " << path << std::endl;
        }
    }

    EmbreeTriangleMesh::~EmbreeTriangleMesh()
    {
        Release();
    }

    void EmbreeTriangleMesh::Release()
    {
        if (m_Geometry)
        {
            rtcReleaseBuffer(m_VertexPositionBuffer);
            rtcReleaseBuffer(m_IndexBuffer);
            rtcReleaseGeometry(m_Geometry);
        }
    }

}   // namespace Hikari