#include "Interaction.h"

#include "Shape.h"

namespace Hikari
{

Interaction::Interaction(
    const glm::vec3&    hitPoint,
    const glm::vec2&    uv,
    const glm::vec3&    dpdu,
    const glm::vec3&    dpdv,
    const glm::vec3&    dndu,
    const glm::vec3&    dndv,
    const glm::vec3&    albedo,
    const Shape*        shape) 
    : m_HitPoint(hitPoint), m_Normal(glm::normalize(glm::cross(dpdv, dpdu))), m_UV(uv),
    m_dpdu(dpdu), m_dpdv(dpdv), m_dndu(dndu), m_dndv(dndv), m_Shape(shape)
{
    Shading.m_Normal = m_Normal;
    Shading.m_dpdu = m_dpdu;
    Shading.m_dpdv = m_dpdv;
    Shading.m_dndu = m_dndu;
    Shading.m_dndv = m_dndv;
    Shading.m_Albedo = albedo;

    if (m_Shape && (m_Shape->m_ReverseOrientation ^ m_Shape->m_TransformSwapsHandedness))
    {
        m_Normal *= -1.f;
        Shading.m_Normal *= -1.f;
    }
    
}

// TODO(achal): Implement this method. PBRT Section 2.10.1 Surface Interaction
void Interaction::SetShadingGeometry(bool shadingNormalIsAuthoritative)
{
    // TO BE IMPLEMENTED LATER
}

}   // namespace Hikari