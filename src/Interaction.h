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
        const glm::vec2&    uv,
        const glm::vec3&    dpdu,
        const glm::vec3&    dpdv,
        const glm::vec3&    dndu,
        const glm::vec3&    dndv,
        const glm::vec3&    albedo,
        const Shape*        shape);

    // Shading Normal and Geometric Normal should always lie in the same hemisphere.
    // The bool value `shadingNormalIsAuthoritative` determines which (if any) normal
    // needs to be flipped so that both Shading Normal and Geometric Normal lie in the
    // correct hemisphere.
    void SetShadingGeometry(bool shadingNormalIsAuthoritative);

    glm::vec3 m_HitPoint;
    glm::vec3 m_Normal;
    glm::vec2 m_UV;
    glm::vec3 m_dpdu, m_dpdv;
    glm::vec3 m_dndu, m_dndv;
    const Shape* m_Shape = nullptr;
    struct
    {
        glm::vec3 m_Normal;
        glm::vec3 m_dpdu, m_dpdv;
        glm::vec3 m_dndu, m_dndv;
        glm::vec3 m_Albedo;
    } Shading;
};

/*
class Interaction
{
public:
    Interaction() {}
    Interaction(
        const glm::vec3&    hitPoint,
        const glm::vec2&    uv,
        const glm::vec3&    dpdu,
        const glm::vec3&    dpdv,
        const glm::vec3&    dndu,
        const glm::vec3&    dndv,
        const Shape*        shape);

    // Shading Normal and Geometric Normal should always lie in the same hemisphere.
    // The bool value `shadingNormalIsAuthoritative` determines which (if any) normal
    // needs to be flipped so that both Shading Normal and Geometric Normal lie in the
    // correct hemisphere.
    void SetShadingGeometry(bool shadingNormalIsAuthoritative);

    glm::vec3 m_HitPoint;
    glm::vec3 m_Normal;
    glm::vec2 m_UV;
    glm::vec3 m_dpdu, m_dpdv;
    glm::vec3 m_dndu, m_dndv;
    const Shape* m_Shape = nullptr;
    struct
    {
        glm::vec3 m_Normal;
        glm::vec3 m_dpdu, m_dpdv;
        glm::vec3 m_dndu, m_dndv;
        glm::vec3 m_Albedo;
    } m_Shading;
};
*/

}   // namespace Hikari