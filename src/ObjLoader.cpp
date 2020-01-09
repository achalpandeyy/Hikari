#include "ObjLoader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <iostream>
#include <string>

namespace Hikari
{

bool LoadObj(
    const char*                 path,
    size_t&                     numVertices,
    size_t&                     numTriangles,
    std::vector<size_t>&        vertexIndices,
    std::vector<size_t>&        normalIndices,
    std::vector<glm::vec3>&     vertexPositions,
    std::vector<glm::vec3>&     vertexNormals)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warning, error;

    bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warning, &error, path);

    if (!warning.empty())
    {
        std::cout << "WARNING: [ " << warning << " ]" << std::endl;
    }
    if (!error.empty())
    {
        std::cout << "ERROR: [ " << error << " ]" << std::endl;
    }

    if (!success)
    {
        return false;
    }

    numVertices = attrib.vertices.size() / 3;

    // Get vertexPositions
    for (size_t i = 0; i < attrib.vertices.size() / 3; ++i)
    {
        vertexPositions.push_back(glm::vec3
        (
            attrib.vertices[3 * i + 0],
            attrib.vertices[3 * i + 1],
            attrib.vertices[3 * i + 2]
        ));
    }

    // Get vertexNormals
    for (size_t i = 0; i < attrib.normals.size() / 3; ++i)
    {
        vertexNormals.push_back(glm::vec3
        (
            attrib.normals[3 * i + 0],
            attrib.normals[3 * i + 1],
            attrib.normals[3 * i + 2]
        ));
    }

    // For each shape in `shapes`
    for (size_t s = 0; s < shapes.size(); ++s)
    {
        size_t idxOffset = 0;
        // For each face in `shapes[s].mesh`
        size_t numFaces = shapes[s].mesh.num_face_vertices.size();
        for (size_t f = 0; f < numFaces; ++f)
        {
            // Number of vertices on this face
            unsigned int numFaceVertices = shapes[s].mesh.num_face_vertices[f];

            for (size_t v = 0; v < numFaceVertices; ++v)
            {
                tinyobj::index_t idx = shapes[s].mesh.indices[idxOffset + v];

                vertexIndices.push_back(idx.vertex_index);
                normalIndices.push_back(idx.normal_index);
            }
            idxOffset += numFaceVertices;
        }
    }
    numTriangles = vertexIndices.size() / 3;

    return true;
}

}   // namespace Hikari