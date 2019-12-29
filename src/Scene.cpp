#include "Scene.h"
#include "Ray.h"

namespace Hikari
{

bool Scene::Intersect(const Ray& ray) const
{
    for (size_t i = 0; i < m_Spheres.size(); ++i)
    {
        float t;
        if (m_Spheres[i]->Intersect(ray, &t) && t < ray.m_MaxDistance)
        {
            return true;
        }
    }
    return false;
}

bool Scene::Intersect(const Ray& ray, Intersection& intersection) const
{
    float tMin = std::numeric_limits<float>::max();
    size_t hitObjIdx;
    bool intersectionFound = false;
    for (size_t i = 0; i < m_Spheres.size(); i++)
    {
        float t;
        if (m_Spheres[i]->Intersect(ray, &t) && t < tMin)
        {
            intersectionFound = true;
            hitObjIdx = i;
            tMin = t;
        }
    }

    if (intersectionFound)
    {
        // Assemble intersection structure
        intersection.m_Valid = true;
        intersection.m_Distance = tMin;

        glm::vec3 hitPoint = ray.m_Origin + ray.m_Direction * intersection.m_Distance;

        intersection.m_Normal = m_Spheres[hitObjIdx]->GetSurfaceNormal(hitPoint);
        intersection.m_TextureCoordinates = m_Spheres[hitObjIdx]->GetTextureCoordinates(hitPoint);
        intersection.m_Albedo = m_Spheres[hitObjIdx]->m_Albedo;
        return true;
    }

    intersection.m_Valid = false;
    return false;
}

}   // namespace Hikari