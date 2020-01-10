#include "Scene.h"

#include "Interaction.h"
#include "Ray.h"

namespace Hikari
{

bool Scene::Intersect(const Ray& ray) const
{
    for (size_t i = 0; i < m_Primitives.size(); ++i)
    {
        if (m_Primitives[i]->IntersectP(ray))
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
    for (size_t i = 0; i < m_Primitives.size(); i++)
    {
        if (m_Primitives[i]->Intersect(ray, intr))
        {
            intersectionFound = true;
            interaction = intr;
        }
    }

    return intersectionFound;
}

}   // namespace Hikari