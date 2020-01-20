#include "ObjLoader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <iostream>
#include <map>
#include <string>

namespace Hikari
{

    struct Less
    {
        bool operator()(const glm::uvec3& a, const glm::uvec3& b) const
        {
            return a.x < b.x || (a.x == b.x && a.y < b.y) || (a.x == b.x && a.y == b.y && a.z < b.z);
        }
    };

    bool LoadObj(
        const char*                 path,
        std::vector<glm::vec3>&     vertexPositions,
        std::vector<glm::vec3>&     vertexNormals,
        std::vector<glm::vec2>&     vertexUV,
        std::vector<glm::uvec3>&    indices,
        const Transform&            objToWorld)
    {
        // TODO(achal): Add support for materials
        std::cout << "Loading OBJ at " << path << std::endl;

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::string warning, error;

        bool success = tinyobj::LoadObj(&attrib, &shapes, nullptr, &warning, &error, path);

        if (!warning.empty())
        {
            std::cout << "tinyobjloader Warning: " << warning << std::endl;
        }

        if (!success)
        {
            if (!error.empty())
                std::cerr << "tinyobjloader Error: " << error << std::endl;
            return false;
        }

        // Structure of Data (Indexed Triangle Mesh):
        // 1. std::vector<glm::vec3> `m_VertexPositions` - collection of unique vertex positions
        // 2. std::vector<glm::vec3> `m_VertexNormals` - collection of unique vertex normals
        // 3. std::vector<glm::vec2> `m_VertexUV` - collection of unique texture coordinates
        // 4. std::vector<glm::uvec3> `m_Indices` - index of vertices which form a triangle
        // One element of the `m_Indices` array denotes a collection of 3 `unsigned int`s which
        // are the indices of the triangle to be formed.
        // Using `m_Indices[i].x` as in index to any of the 1, 2, 3 should give the attribute
        // of the same vertex.
        //
        // We are currently using a single vertex position, normal and uv array for all the
        // shapes contained in the `.obj` file. For example, if we load a `CornellBox.obj`
        // there might be more than one shape(leftWall, rightWall, roof, floor, box1.. etc).
        // All of these shapes can have their separate position, normal and uv arrays, but
        // for now we have chosen to append them one after the other in the same array;

        for (tinyobj::shape_t shape : shapes)
        {
            const tinyobj::mesh_t& mesh = shape.mesh;

            std::map<glm::uvec3, unsigned int, Less> idxMapping;

            size_t numFaces = mesh.num_face_vertices.size();
            for (size_t f = 0; f < numFaces; ++f)
            {
                // Since, we're triangluating the mesh, we should have
                // all triangle faces.
                assert(mesh.num_face_vertices[f] == 3u);

                glm::uvec3 triangleIndices;
                for (size_t i = 0; i < 3u; ++i)
                {
                    const glm::uvec3 idx
                    (
                        mesh.indices[3 * f + i].vertex_index,
                        mesh.indices[3 * f + i].normal_index,
                        mesh.indices[3 * f + i].texcoord_index
                    );

                    unsigned int vertIdx = 0u;

                    // Map the tinyobjloader's (position, normal, uv) index format to
                    // a single index which could be used to access data for all three
                    // arrays (position, normal and uv).
                    // `[i1, i2, i3]` gets mapped to `i`.
                    std::map<glm::uvec3, unsigned int>::iterator result = idxMapping.find(idx);
                    if (result != idxMapping.end())
                    {
                        vertIdx = idxMapping[idx];
                    }
                    else
                    {
                        vertIdx = vertexPositions.size();
                        idxMapping[idx] = vertIdx;

                        // Vertex Positions
                        vertexPositions.push_back(objToWorld.TransformPoint(glm::vec3
                        (
                            attrib.vertices[3 * idx.x],
                            attrib.vertices[3 * idx.x + 1],
                            attrib.vertices[3 * idx.x + 2]
                        )));

                        // Vertex Normals
                        if (idx.y != (unsigned int)(-1))
                        {
                           vertexNormals.push_back(glm::normalize(
                                objToWorld.TransformNormal(glm::vec3
                                (
                                attrib.normals[3u * idx.y],
                                attrib.normals[3u * idx.y + 1u],
                                attrib.normals[3u * idx.y + 2u]
                                ))));
                        }

                        // Vertex UV
                        if (idx.z != (unsigned int)(-1))
                        {
                            vertexUV.emplace_back
                            (
                                attrib.texcoords[2u * idx.z],
                                attrib.texcoords[2u * idx.z + 1u]
                            );
                        }
                    }
                    triangleIndices[i] = vertIdx;
                }
                indices.push_back(triangleIndices);
            }
        }
    }

}   // namespace Hikari