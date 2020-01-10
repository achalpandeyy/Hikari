#include "Primitive.h"

#include "Interaction.h"
#include "Ray.h"

namespace Hikari
{

bool GeometricPrimitive::Intersect(const Ray& ray, Interaction& interaction) const
{
    float tHit;
    if (m_Shape->Intersect(ray, tHit, interaction))
    {
        ray.m_tMax = tHit;
        interaction.m_Primitive = this;
        return true;
    }
    return false;
}

bool GeometricPrimitive::IntersectP(const Ray& ray) const
{
    return m_Shape->IntersectP(ray);
}

}   // namespace Hikari