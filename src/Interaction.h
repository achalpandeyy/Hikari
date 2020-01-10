#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

namespace Hikari
{

class Primitive;
class Shape;

class Interaction
{
public:
    Interaction() {}
    Interaction(
        const glm::vec3&    hitPoint,
        const glm::vec3&    normal,
        const Shape*        shape);

    glm::vec3 m_HitPoint;
    glm::vec3 m_Normal;
    const Shape* m_Shape = nullptr;
    const Primitive* m_Primitive = nullptr;
};

}   // namespace Hikari