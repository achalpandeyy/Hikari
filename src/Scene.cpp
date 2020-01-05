#include "Scene.h"

#include "Interaction.h"
#include "Ray.h"

namespace Hikari
{

bool Scene::Intersect(const Ray& ray) const
{
    Interaction interaction;
    for (size_t i = 0; i < m_Spheres.size(); ++i)
    {
        float t;
        if (m_Spheres[i]->IntersectP(ray) && t < ray.m_tMax)
        {
            return true;
        }
    }
    return false;
}

bool Scene::Intersect(const Ray& ray, Interaction& interaction) const
{
    Interaction intr;
    float tMin = std::numeric_limits<float>::max();
    bool intersectionFound = false;
    for (size_t i = 0; i < m_Spheres.size(); i++)
    {
        float t;
        if (m_Spheres[i]->Intersect(ray, &t, &intr) && t < tMin)
        {
            intersectionFound = true;
            interaction = intr;
            tMin = t;
        }
    }

    return intersectionFound;
}

}   // namespace Hikari