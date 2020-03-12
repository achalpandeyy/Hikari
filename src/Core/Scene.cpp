#include "Scene.h"

#include "Core/Interaction.h"
#include "Math/Ray.h"

namespace Hikari
{
    bool Scene::Intersect(const Ray& ray, Interaction* interaction) const
    {
        // Convert Hikari's conception of ray to RTCRay or RTCRayHit
        //
        // Probably we don't need a RTCIntersectContext right now,
        // trying passing nullptr in the corresponding field in rtcIntersect1
        //
        RTCIntersectContext context;
        rtcInitIntersectContext(&context);

        RTCRayHit rayhit;
        rayhit.ray = ToRTCRay(ray);
        rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
        rayhit.hit.instID[0] = RTC_INVALID_GEOMETRY_ID;

        // Intersect a single ray with the scene
        rtcIntersect1(m_RTCScene, &context, &rayhit);

        if (rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID)
        {
            assert(m_Primitives.size() > rayhit.hit.geomID);

            *interaction = Interaction(
                ray(rayhit.ray.tfar),
                glm::normalize(glm::vec3(rayhit.hit.Ng_x, rayhit.hit.Ng_y, rayhit.hit.Ng_z)),
                -ray.m_Direction,
                glm::vec2(rayhit.hit.u, rayhit.hit.v),
                m_Primitives[rayhit.hit.geomID].get());

            return true;
        }
        return false;
    }

    bool Scene::Occluded(const Ray& ray) const
    {
        RTCIntersectContext context;
        rtcInitIntersectContext(&context);

        RTCRay rtcRay = ToRTCRay(ray);

        rtcOccluded1(m_RTCScene, &context, &rtcRay);

        return rtcRay.tfar == -INFINITY;
    }

    // TODO(achal): Put it in Ray.
    RTCRay Scene::ToRTCRay(const Ray& ray) const
    {
        RTCRay rtcRay;
        rtcRay.org_x = ray.m_Origin.x;
        rtcRay.org_y = ray.m_Origin.y;
        rtcRay.org_z = ray.m_Origin.z;
        rtcRay.dir_x = ray.m_Direction.x;
        rtcRay.dir_y = ray.m_Direction.y;
        rtcRay.dir_z = ray.m_Direction.z;
        rtcRay.tnear = 0;
        rtcRay.tfar = ray.m_tMax;
        rtcRay.mask = 0u;
        rtcRay.flags = 0u;
        return rtcRay;
    }

}   // namespace Hikari