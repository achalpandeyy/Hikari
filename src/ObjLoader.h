#pragma once

#include "glm/vec3.hpp"

#include <vector>

namespace Hikari
{

bool LoadObj(
    const char*                 path,
    size_t&                     numVertices,
    size_t&                     numTriangles,
    std::vector<size_t>&        vertexIndices,
    std::vector<size_t>&        normalIndices,
    std::vector<glm::vec3>&     vertexPositions,
    std::vector<glm::vec3>&     vertexNormals);

}   // namespace Hikari
