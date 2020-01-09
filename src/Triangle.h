#pragma once

#include "Shape.h"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include <memory>
#include <vector>

namespace Hikari
{

class Interaction;
class Ray;
class Transform;

struct TriangleMesh
{
    TriangleMesh(
        const Transform&                objectToWorld,
        const size_t&                   numVertices,
        const size_t&                   numTriangles,
        const std::vector<size_t>&      vertexIndices,
        const std::vector<size_t>&      normalIndices,
        const std::vector<glm::vec3>&   vertexPositions,
        const std::vector<glm::vec3>&   vertexNormals);

    const int m_NumVertices, m_NumTriangles;
    std::vector<size_t> m_VertexIndices, m_NormalIndices;
    std::vector<glm::vec3> m_VertexPositions, m_VertexNormals;
};

class Triangle : public Shape
{
public:
    Triangle(
        const Transform*                        objectToWorld,
        const Transform*                        worldToObject,
        const glm::vec3&                        albedo,
        bool                                    reverseOrientation,
        const std::shared_ptr<TriangleMesh>&    mesh,
        int                                     triNumber);

    bool Intersect(const Ray& ray, float* t, Interaction* interaction) const;
    bool IntersectP(const Ray& ray) const;

private:
    std::shared_ptr<TriangleMesh> m_Mesh;
    const size_t* m_Vertices;
    const size_t* m_Normals;
};

std::vector< std::shared_ptr<Shape> > CreateTriangleMesh(
    const char*         path,
    Transform*          objectToWorld,
    Transform*          worldToObject,
    const glm::vec3&    albedo);

}   // namespace Hikari
