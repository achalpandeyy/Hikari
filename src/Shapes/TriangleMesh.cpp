#include "TriangleMesh.h"

#include "ObjLoader.h"

#include <iostream>

namespace Hikari
{
    TriangleMesh::TriangleMesh(
        const char*         path,
        const Transform&    objectToWorld,
        RTCDevice           device,
        RTCScene            scene)
        : Shape(rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE))
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

            m_Id = rtcAttachGeometry(scene, m_Geometry);
        }
        else
        {
            std::cerr << "Failed to load model at path " << path << std::endl;
        }
    }

    TriangleMesh::~TriangleMesh()
    {
        if (m_Geometry)
        {
            rtcReleaseBuffer(m_VertexPositionBuffer);
            rtcReleaseBuffer(m_IndexBuffer);
            rtcReleaseGeometry(m_Geometry);
        }
    }

    std::shared_ptr<TriangleMesh> CreateTriangleMeshShape(
        const char*         path,
        const glm::vec3&    position,
        float               degrees,
        const glm::vec3&    rotationAxis,
        const glm::vec3&    scale,
        RTCDevice           rtcDevice,
        RTCScene            rtcScene)
    {
        Transform objectToWorld = Translate(position);
        objectToWorld = objectToWorld * Rotate(degrees, rotationAxis);
        objectToWorld = objectToWorld * Scale(scale);

        return std::make_shared<TriangleMesh>(path, objectToWorld, rtcDevice, rtcScene);
    }

}   // namespace Hikari