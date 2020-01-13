#include "AABB.h"

namespace Hikari
{

const glm::vec3& AABB::operator [] (int i) const
{
    assert(i == 0 || i == 1);
    return i ? m_PointMax : m_PointMin;
}

glm::vec3& AABB::operator [] (int i)
{
    assert(i == 0 || i == 1);
    return i ? m_PointMax : m_PointMin;
}

glm::vec3 AABB::Corner(int corner) const
{
    return glm::vec3
    (
        (*this)[corner & 1].x,
        (*this)[(corner & 2) ? 1 : 0].y,
        (*this)[(corner & 4) ? 1 : 0].z
    );
}

glm::vec3 AABB::Diagonal() const
{
    return m_PointMax - m_PointMin;
}

float AABB::SurfaceArea() const
{
    glm::vec3 d = Diagonal();
    return 2.f * (d.x * d.y + d.x * d.z + d.y * d.z);
}

float AABB::Volume() const
{
    glm::vec3 d = Diagonal();
    return d.x * d.y * d.z;
}

int AABB::MaxExtent() const
{
    glm::vec3 d = Diagonal();
    if (d.x > d.y && d.x > d.z)
    {
        return 0;
    }
    else if (d.y > d.z)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

AABB Union(const AABB& box, const glm::vec3& point)
{
    return AABB
    (
        glm::min(box.m_PointMin, point),
        glm::max(box.m_PointMax, point)
    );
}

AABB Union(const AABB& box1, const AABB& box2)
{
    return AABB
    (
        glm::min(box1.m_PointMin, box2.m_PointMin),
        glm::max(box1.m_PointMax, box2.m_PointMax)
    );
}

AABB Intersection(const AABB& box1, const AABB& box2)
{
    return AABB
    (
        glm::max(box1.m_PointMin, box2.m_PointMin),
        glm::min(box1.m_PointMax, box2.m_PointMax)
    );
}

bool Overlaps(const AABB& box1, const AABB& box2)
{
    bool x = (box1.m_PointMax.x >= box2.m_PointMin.x) && (box1.m_PointMin.x <= box2.m_PointMax.x);
    bool y = (box1.m_PointMax.y >= box2.m_PointMin.y) && (box1.m_PointMin.y <= box2.m_PointMax.y);
    bool z = (box1.m_PointMax.z >= box2.m_PointMin.z) && (box1.m_PointMin.z <= box2.m_PointMax.z);
    return x && y && z;
}

bool Inside(const glm::vec3& point, const AABB& box)
{
    bool x = box.m_PointMin.x <= point.x && box.m_PointMax.x >= point.x;
    bool y = box.m_PointMin.y <= point.y && box.m_PointMax.y >= point.y;
    bool z = box.m_PointMin.z <= point.z && box.m_PointMax.z >= point.z;
    return x && y && z;
}

bool InsideExclusive(const glm::vec3& point, const AABB& box)
{
    bool x = box.m_PointMin.x <= point.x && box.m_PointMax.x > point.x;
    bool y = box.m_PointMin.y <= point.y && box.m_PointMax.y > point.y;
    bool z = box.m_PointMin.z <= point.z && box.m_PointMax.z > point.z;
    return x && y && z;
}

AABB Expand(const AABB& box, float delta)
{
    return AABB
    (
        glm::vec3(box.m_PointMin - glm::vec3(delta)),
        glm::vec3(box.m_PointMax + glm::vec3(delta))
    );
}

}   // namespace Hikari