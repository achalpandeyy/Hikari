#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

namespace Hikari
{

class Shape;

class Interaction
{
public:
    Interaction() {}
    Interaction(
        const glm::vec3&    hitPoint,
        const glm::vec3&    normal,
        const glm::vec3&    albedo,
        const Shape*        shape);

    glm::vec3 m_HitPoint;
    glm::vec3 m_Normal;
    glm::vec3 m_Albedo;
    const Shape* m_Shape = nullptr;
};

}   // namespace Hikari