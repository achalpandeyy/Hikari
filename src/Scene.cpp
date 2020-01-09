#include "Scene.h"

#include "Interaction.h"
#include "Ray.h"

namespace Hikari
{

bool Scene::Intersect(const Ray& ray) const
{
    for (size_t i = 0; i < m_Shapes.size(); ++i)
    {
        if (m_Shapes[i]->IntersectP(ray))
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
    for (size_t i = 0; i < m_Shapes.size(); i++)
    {
        float t;
        if (m_Shapes[i]->Intersect(ray, &t, &intr) && t < tMin)
        {
            intersectionFound = true;
            interaction = intr;
            tMin = t;
        }
    }

    return intersectionFound;
}

}   // namespace Hikari