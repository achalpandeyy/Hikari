#include "Triangle.h"

#include "Interaction.h"
#include "ObjLoader.h"
#include "Ray.h"
#include "Transform.h"

#include <iostream>

namespace Hikari
{

TriangleMesh::TriangleMesh(
    const Transform&                objectToWorld,
    const size_t&                   numVertices,
    const size_t&                   numTriangles,
    const std::vector<size_t>&      vertexIndices,
    const std::vector<size_t>&      normalIndices,
    const std::vector<glm::vec3>&   vertexPositions,
    const std::vector<glm::vec3>&   vertexNormals)
    : m_NumVertices(numVertices), m_NumTriangles(numTriangles),
    m_VertexIndices(vertexIndices), m_NormalIndices(normalIndices)
{
    m_VertexPositions.resize(vertexPositions.size());
    for (size_t i = 0; i < vertexPositions.size(); ++i)
    {
        m_VertexPositions[i] = objectToWorld.TransformPoint(vertexPositions[i]);
    }

    m_VertexNormals.resize(vertexNormals.size());
    for (size_t i = 0; i < vertexNormals.size(); ++i)
    {
        m_VertexNormals[i] = objectToWorld.TransformNormal(vertexNormals[i]);
    }
}

Triangle::Triangle(
    const Transform*                        objectToWorld,
    const Transform*                        worldToObject,
    bool                                    reverseOrientation,
    const std::shared_ptr<TriangleMesh>&    mesh,
    int                                     triNumber)
    : Shape(objectToWorld, worldToObject, reverseOrientation), m_Mesh(mesh)
{
    m_Vertices = &m_Mesh->m_VertexIndices[3 * triNumber];
    m_Normals = &m_Mesh->m_NormalIndices[3 * triNumber];
}

bool Triangle::Intersect(const Ray& ray, float& t, Interaction& interaction) const
{
    const glm::vec3& p0 = m_Mesh->m_VertexPositions[m_Vertices[0]];
    const glm::vec3& p1 = m_Mesh->m_VertexPositions[m_Vertices[1]];
    const glm::vec3& p2 = m_Mesh->m_VertexPositions[m_Vertices[2]];

    // Perform Ray-Triangle intersection test
    // Transform Triangle vertices to Ray coordinate space
    glm::vec3 p0t = p0 - ray.m_Origin;
    glm::vec3 p1t = p1 - ray.m_Origin;
    glm::vec3 p2t = p2 - ray.m_Origin;

    // TODO(achal): The following operations of Permutation and Shear Transformation
    // are independent of the triangle and only depends upon the ray. For better perf
    // this information (required to apply permutation and shearing) can be stored in
    // the `Ray` class.

    // Permute components of triangle vertices and ray direction
    glm::vec3 absRayDir = glm::abs(ray.m_Direction);
    int kz = absRayDir.x > absRayDir.y
        ? (absRayDir.x > absRayDir.z ? 0 : 2)
        : (absRayDir.y > absRayDir.z ? 1 : 2);
    int kx = (kz + 1) % 3;
    int ky = (kx + 1) % 3;
    glm::vec3 rayDir(ray.m_Direction[kx], ray.m_Direction[ky], ray.m_Direction[kz]);
    p0t = glm::vec3(p0t[kx], p0t[ky], p0t[kz]);
    p1t = glm::vec3(p1t[kx], p1t[ky], p1t[kz]);
    p2t = glm::vec3(p2t[kx], p2t[ky], p2t[kz]);

    // Apply shear transformation only on the x and y coordinates for now.
    // We can apply that on the z coordinate as well if the ray intersects the triangle.
    float sx = -rayDir.x / rayDir.z;
    float sy = -rayDir.y / rayDir.z;
    float sz = 1.f / rayDir.z;
    p0t.x += sx * p0t.z;
    p0t.y += sy * p0t.z;
    p1t.x += sx * p1t.z;
    p1t.y += sy * p1t.z;
    p2t.x += sx * p2t.z;
    p2t.y += sy * p2t.z;

    // Compute edge function coefficients
    float e0 = p1t.x * p2t.y - p2t.x * p1t.y;
    float e1 = p2t.x * p0t.y - p2t.y * p0t.x;
    float e2 = p0t.x * p1t.y - p0t.y * p1t.x;

    // TODO(achal): In the case when any of the coefficients (e0, e1, e2) become
    // 0, we should fall back to `double` precision and recompute them.
    // Reference PBRT Section 3.9.3 for this.

    if ((e0 < 0.f || e1 < 0.f || e2 < 0.f) && (e0 > 0.f || e1 > 0.f || e2 > 0.f))
    {
        // Since the point in question (origin (x = 0, y = 0)) didn't give same
        // sign with all the three edges it means it doesn't lie inside the triangle
        return false;
    }

    float det = e0 + e1 + e2;
    if (det == 0.f)
    {
        return false;
    }

    // First finish converting z coordinates to ray-triangle coordinate system
    p0t.z *= sz;
    p1t.z *= sz;
    p2t.z *= sz;

    float tScaled = p0t.z * e0 + p1t.z * e1 + p2t.z * e2;
    if (det < 0.f && (tScaled >= 0.f || tScaled < ray.m_tMax * det))
    {
        return false;
    }
    else if (det > 0.f && (tScaled <= 0.f || tScaled > ray.m_tMax * det))
    {
        return false;
    }

    // Compute barycentric coordinates
    float invDet = 1.f / det;
    float b0 = e0 * invDet;
    float b1 = e1 * invDet;
    float b2 = e2 * invDet;
    float tHit = tScaled * invDet;

    /*
    // Compute partial derivatives
    glm::vec3 dpdu, dpdv;
    glm::vec2 uv[3];
    GetUVs(uv);

    glm::vec2 diffUV02 = uv[0] - uv[2], diffUV12 = uv[1] - uv[2];
    glm::vec3 diffP02 = p0 - p2, diffP12 = p1 - p2;

    float determinant = diffUV02[0] * diffUV12[1] - diffUV02[1] * diffUV12[0];
    bool degenerateUV = determinant < 1e-8;

    if (!degenerateUV)
    {
        float invDet = 1.f / determinant;
        dpdu = (diffUV12[1] * diffP02 - diffUV02[1] * diffP12) * invDet;
        dpdv = (-diffUV12[0] * diffP02 + diffUV02[0] * diffP12) * invDet;
    }

    glm::vec3 normal = glm::cross(dpdv, dpdu);
    if (degenerateUV || glm::dot(normal, normal) == 0.f)
    {
        glm::vec3 geomNormal = glm::cross(p1 - p0, p2 - p0);
        if (glm::dot(geomNormal, geomNormal) == 0.f)
        {
            // Bogus triangle
            return false;
        }

        // Choose an arbitrary coordinate system about triangle's surface normal
        // making sure it is orthonormal.

        // TODO(achal): Consider putting it in its own function somewhere else
        geomNormal = glm::normalize(geomNormal);
        if (std::abs(normal.x) > std::abs(normal.y))
        {
            dpdu = glm::normalize(glm::vec3(-normal.z, 0.f, normal.x));
        }
        else
        {
            dpdu = glm::normalize(glm::vec3(0.f, normal.z, -normal.y));
        }
        dpdv = glm::cross(dpdu, geomNormal);
    }
    
    glm::vec3 pHit = b0 * p0 + b1 * p1 + b2 * p2;
    glm::vec2 uvHit = b0 * uv[0] + b1 * uv[1] + b2 * uv[2];

    // Fill up `Interaction`
    // `dndu` and `dndv` will be (0, 0, 0) because triangle has a flat surface,
    // so the geometric normal doesn't change with u or v.
    *interaction = Interaction(pHit, uvHit, dpdu, dpdv, glm::vec3(0.f),
        glm::vec3(0.f), glm::vec3(0.f, 1.f, 1.f), this);

    // The reason we need to set the normal manually here is because it is more
    // reliable rather than delegating this calculation in the constructor which
    // essentially crosses `dpdv` with `dpdu` to get the normal. Often times,
    // the parametric data of a mesh is not very reliable so for the sake of
    // robustness we compute the normal manually, here.
    interaction->m_Normal = interaction->Shading.m_Normal = glm::normalize(glm::cross(diffP12, diffP02));
    if (m_ReverseOrientation ^ m_TransformSwapsHandedness)
    {
        interaction->m_Normal = interaction->Shading.m_Normal = -interaction->m_Normal;
    }

    if (m_Mesh->m_VertexNormals || m_Mesh->m_VertexTangents)
    {
        // Compute shading normal
        glm::vec3 shNormal;
        if (m_Mesh->m_VertexNormals)
        {
            shNormal = glm::normalize
            (
                b0 * m_Mesh->m_VertexNormals[m_Vertices[0]] +
                b1 * m_Mesh->m_VertexNormals[m_Vertices[1]] +
                b2 * m_Mesh->m_VertexNormals[m_Vertices[2]]
            );

            if (glm::dot(shNormal, shNormal) > 0.f)
            {
                shNormal = glm::normalize(shNormal);
            }
            else
            {
                shNormal = interaction->m_Normal;
            }
        }
        else
        {
            shNormal = interaction->m_Normal;
        }

        // Compute shading tangent
        glm::vec3 shTangent;
        if (m_Mesh->m_VertexTangents)
        {
            shTangent = glm::normalize
            (
                b0 * m_Mesh->m_VertexTangents[m_Vertices[0]] +
                b1 * m_Mesh->m_VertexTangents[m_Vertices[1]] +
                b2 * m_Mesh->m_VertexTangents[m_Vertices[2]]
            );

            if (glm::dot(shTangent, shTangent) > 0.f)
            {
                shTangent = glm::normalize(shTangent);
            }
            else
            {
                shTangent = glm::normalize(interaction->m_dpdu);
            }
        }
        else
        {
            shTangent = glm::normalize(interaction->m_dpdu);
        }

        // Compute shading bitangent
        glm::vec3 shBitangent = glm::cross(shNormal, shTangent);
        if (glm::dot(shBitangent, shBitangent) > 0.f)
        {
            shBitangent = glm::normalize(shBitangent);

            // Readjust the shading tangent because interpolated normals
            // and tangents may not be perfectly perpendicular.
            shTangent = glm::cross(shNormal, shBitangent);
        }
        else
        {
            // Create a coordinate system around `shNormal`.
            if (std::abs(shNormal.x) > std::abs(shNormal.y))
            {
                shTangent = glm::normalize(glm::vec3(-shNormal.z, 0.f, shNormal.x));
            }
            else
            {
                shTangent = glm::normalize(glm::vec3(0.f, shNormal.z, -shNormal.y));
            }
            shBitangent = glm::cross(shTangent, shNormal);
        }

        // Compute `dndu` and `dndv`
        glm::vec3 dndu, dndv;
        if (m_Mesh->m_VertexNormals)
        {
            // glm::vec2 diffUV02 = uv[0] - uv[2], diffUV12 = uv[1] - uv[2];
            glm::vec3 diffN02 = m_Mesh->m_VertexNormals[m_Vertices[0]] - m_Mesh->m_VertexNormals[m_Vertices[2]];
            glm::vec3 diffN12 = m_Mesh->m_VertexNormals[m_Vertices[1]] - m_Mesh->m_VertexNormals[m_Vertices[2]];

            // float determinant = diffUV02[0] * diffUV12[1] - diffUV02[1] * diffUV12[0];
            // bool degenerateUV = std::abs(determinant) < 1e-8;

            if (degenerateUV)
            {

            }
            else
            {
                // float invDet = 1.f / determinant;
                dndu = (diffUV12[1] * diffN02 - diffUV02[1] * diffN12) * invDet;
                dndv = (-diffUV12[0] * diffN02 + diffUV02[0] * diffN12) * invDet;
            }
        }
        else
        {
            dndu = dndv = glm::vec3(0.f);
        }

        if (m_ReverseOrientation)
        {
            shTangent = -shTangent;
        }
        interaction->SetShadingGeometry();
    }
    */

    interaction = Interaction(ray(tHit), glm::normalize(glm::cross(p1 - p0, p2 - p0)), this);
    t = tHit;
    return true;
}

bool Triangle::IntersectP(const Ray& ray) const
{
    const glm::vec3& p0 = m_Mesh->m_VertexPositions[m_Vertices[0]];
    const glm::vec3& p1 = m_Mesh->m_VertexPositions[m_Vertices[1]];
    const glm::vec3& p2 = m_Mesh->m_VertexPositions[m_Vertices[2]];

    // Perform Ray-Triangle intersection test
    // Transform Triangle vertices to Ray coordinate space
    glm::vec3 p0t = p0 - ray.m_Origin;
    glm::vec3 p1t = p1 - ray.m_Origin;
    glm::vec3 p2t = p2 - ray.m_Origin;

    // TODO(achal): The following operations of Permutation and Shear Transformation
    // are independent of the triangle and only depends upon the ray. For better perf
    // this information (required to apply permutation and shearing) can be stored in
    // the `Ray` class.

    // Permute components of triangle vertices and ray direction
    glm::vec3 absRayDir = glm::abs(ray.m_Direction);
    int kz = absRayDir.x > absRayDir.y
        ? (absRayDir.x > absRayDir.z ? 0 : 2)
        : (absRayDir.y > absRayDir.z ? 1 : 2);
    int kx = (kz + 1) % 3;
    int ky = (kx + 1) % 3;
    glm::vec3 rayDir(ray.m_Direction[kx], ray.m_Direction[ky], ray.m_Direction[kz]);
    p0t = glm::vec3(p0t[kx], p0t[ky], p0t[kz]);
    p1t = glm::vec3(p1t[kx], p1t[ky], p1t[kz]);
    p2t = glm::vec3(p2t[kx], p2t[ky], p2t[kz]);

    // Apply shear transformation only on the x and y coordinates for now.
    // We can apply that on the z coordinate as well if the ray intersects the triangle.
    float sx = -rayDir.x / rayDir.z;
    float sy = -rayDir.y / rayDir.z;
    float sz = 1.f / rayDir.z;
    p0t.x += sx * p0t.z;
    p0t.y += sy * p0t.z;
    p1t.x += sx * p1t.z;
    p1t.y += sy * p1t.z;
    p2t.x += sx * p2t.z;
    p2t.y += sy * p2t.z;

    // Compute edge function coefficients
    float e0 = p1t.x * p2t.y - p2t.x * p1t.y;
    float e1 = p2t.x * p0t.y - p2t.y * p0t.x;
    float e2 = p0t.x * p1t.y - p0t.y * p1t.x;

    // TODO(achal): In the case when any of the coefficients (e0, e1, e2) become
    // 0, we should fall back to `double` precision and recompute them.
    // Reference PBRT Section 3.9.3 for this.

    if ((e0 < 0.f || e1 < 0.f || e2 < 0.f) && (e0 > 0.f || e1 > 0.f || e2 > 0.f))
    {
        // Since the point in question (origin (x = 0, y = 0)) didn't give same
        // sign with all the three edges it means it doesn't lie inside the triangle
        return false;
    }

    float det = e0 + e1 + e2;
    if (det == 0.f)
    {
        return false;
    }

    // First finish converting z coordinates to ray-triangle coordinate system
    p0t.z *= sz;
    p1t.z *= sz;
    p2t.z *= sz;

    float tScaled = p0t.z * e0 + p1t.z * e1 + p2t.z * e2;
    if (det < 0.f && (tScaled >= 0.f || tScaled < ray.m_tMax * det))
    {
        return false;
    }
    else if (det > 0.f && (tScaled <= 0.f || tScaled > ray.m_tMax * det))
    {
        return false;
    }

    return true;
}

std::vector< std::shared_ptr<Shape> > CreateTriangleMesh(
    const char*         path,
    Transform*          objectToWorld,
    Transform*          worldToObject)
{
    size_t numVertices, numTriangles;
    std::vector<size_t> vertexIndices, normalIndices;
    std::vector<glm::vec3> vertexPositions, vertexNormals;

    bool success = LoadObj(path, numVertices, numTriangles, vertexIndices, normalIndices,
        vertexPositions, vertexNormals);

    if (success)
    {
        std::shared_ptr<TriangleMesh> mesh = std::make_shared<TriangleMesh>(
            *objectToWorld, numVertices, numTriangles, vertexIndices, normalIndices,
            vertexPositions, vertexNormals);

        std::vector< std::shared_ptr<Shape> > triangles(numTriangles);

        for (size_t i = 0; i < numTriangles; ++i)
        {
            std::shared_ptr<Shape> triangle = std::make_shared<Triangle>(
                objectToWorld, worldToObject, false, mesh, i);
            triangles[i] = triangle;
        }
        return triangles;
    }
    else
    {
        std::cerr << "Error loading model at " << path << std::endl;
        return std::vector< std::shared_ptr<Shape> >();
    }
}

}   // namespace Hikari