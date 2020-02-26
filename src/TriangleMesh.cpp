#include "Shape.h"
#include "Material.h"
#include "ObjLoader.h"

#include <iostream>

namespace Hikari
{

    TriangleMesh::TriangleMesh(
        RTCDevice                           device,
        const Transform&                    objectToWorld,
        const char*                         path,
        const std::shared_ptr<Material>&    material,
        const glm::vec3&                    emission)
        : Shape(rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE), objectToWorld,
            material, emission)
    {
        // Load the obj file.
        if (LoadObj(path, m_VertexPositions, m_VertexNormals, m_VertexUV, m_Indices,
            m_ObjectToWorld))
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

    TriangleMesh::~TriangleMesh()
    {
        Release();
    }

    void TriangleMesh::Release()
    {
        if (m_Geometry)
        {
            rtcReleaseBuffer(m_VertexPositionBuffer);
            rtcReleaseBuffer(m_IndexBuffer);
            rtcReleaseGeometry(m_Geometry);
        }
    }

}   // namespace Hikari