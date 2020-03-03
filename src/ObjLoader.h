#pragma once

#include "Math/Transform.h"

#include "glm/glm.hpp"

#include <vector>

namespace Hikari
{
    bool LoadObj(
        const char*                 path,
        std::vector<glm::vec3>&     vertexPositions,
        std::vector<glm::vec3>&     vertexNormals,
        std::vector<glm::vec2>&     vertexUV,
        std::vector<glm::uvec3>&    indices,
        const Transform&            objToWorld);

}   // namespace Hikari
