#include "EmbreeGeometry.h"

namespace Hikari
{

    void Bounds(const RTCBoundsFunctionArguments* args);
    void Intersect(const RTCIntersectFunctionNArguments* args);
    void Occluded(const RTCOccludedFunctionNArguments* args);

    EmbreeSphere::EmbreeSphere(
        RTCDevice           device,
        const glm::vec3&    center,
        float               radius,
        const glm::vec3&    albedo)
        : EmbreeGeometry(rtcNewGeometry(device, RTC_GEOMETRY_TYPE_USER), albedo),
        m_Center(center), m_Radius(radius)
    {
        // Set all callbacks
        rtcSetGeometryUserPrimitiveCount(m_Geometry, 1u);
        rtcSetGeometryUserData(m_Geometry, this);

        rtcSetGeometryBoundsFunction(m_Geometry, Bounds, nullptr);
        rtcSetGeometryIntersectFunction(m_Geometry, Intersect);
        rtcSetGeometryOccludedFunction(m_Geometry, Occluded);

        rtcCommitGeometry(m_Geometry);
    }

    EmbreeSphere::~EmbreeSphere()
    {
        Release();
    }

    void EmbreeSphere::Release()
    {
        if (m_Geometry)
            rtcReleaseGeometry(m_Geometry);
    }


    void Bounds(const RTCBoundsFunctionArguments* args)
    {
        const EmbreeSphere* spherePtr = static_cast<const EmbreeSphere*>(
            args->geometryUserPtr);
        const EmbreeSphere& sphere = *spherePtr;

        RTCBounds* bounds_o = args->bounds_o;
        bounds_o->lower_x = sphere.m_Center.x - sphere.m_Radius;
        bounds_o->lower_y = sphere.m_Center.y - sphere.m_Radius;
        bounds_o->lower_z = sphere.m_Center.z - sphere.m_Radius;
        bounds_o->upper_x = sphere.m_Center.x + sphere.m_Radius;
        bounds_o->upper_y = sphere.m_Center.y + sphere.m_Radius;
        bounds_o->upper_z = sphere.m_Center.z + sphere.m_Radius;
    }

    // The implementations of the following two functions `Intersect` and
    // `Occluded` are a stripped down version of the functions in the
    // following file.
    // https://github.com/embree/embree/blob/master/tutorials/user_geometry/user_geometry_device.cpp
    // Many features which are included above are stripped down from here.
    // Refer to the above link for more features to specialize the implementation.
    void Intersect(const RTCIntersectFunctionNArguments* args)
    {
        // Intersect is only implemented for one sphere per User Geometry
        assert(args->N == 1u);

        bool isRayValid = *(args->valid);
        const EmbreeSphere* spherePtr = static_cast<const EmbreeSphere*>(
            args->geometryUserPtr);
        const EmbreeSphere& sphere = *spherePtr;

        if (!isRayValid) return;

        RTCRayHit* rayhit = (RTCRayHit*)(args->rayhit);
        RTCRay* ray = (RTCRay*)(&rayhit->ray);
        RTCHit* hit = (RTCHit*)(&rayhit->hit);

        glm::vec3 rayOrigin = glm::vec3(ray->org_x, ray->org_y, ray->org_z);
        glm::vec3 rayDirection = glm::vec3(ray->dir_x, ray->dir_y, ray->dir_z);

        const glm::vec3 V = rayOrigin - sphere.m_Center;
        const float A = glm::dot(rayDirection, rayDirection);
        const float B = 2.0f * glm::dot(V, rayDirection);
        const float C = glm::dot(V, V) - (sphere.m_Radius * sphere.m_Radius);
        const float D = B * B - 4.0f * A * C;

        if (D < 0.0f) return;

        const float Q = std::sqrt(D);
        const float rcpA = 1.f / A;
        const float t0 = 0.5f * rcpA * (-B - Q);
        const float t1 = 0.5f * rcpA * (-B + Q);

        RTCHit potentialHit;
        potentialHit.u = 0.0f;
        potentialHit.v = 0.0f;

        // TODO(achal): Set sensible values for `geomID` and `primID` and possibly
        // make use of them during shading.
        potentialHit.geomID = sphere.GetId();
        potentialHit.primID = 100u;

        if ((ray->tnear < t0) & (t0 < ray->tfar))
        {
            const glm::vec3 geometricNormal = rayOrigin + t0 * rayDirection
                - sphere.m_Center;

            potentialHit.Ng_x = geometricNormal.x;
            potentialHit.Ng_y = geometricNormal.y;
            potentialHit.Ng_z = geometricNormal.z;

            ray->tfar = t0;
            *hit = potentialHit;
        }

        if ((ray->tnear < t1) & (t1 < ray->tfar))
        {
            const glm::vec3 geometricNormal = rayOrigin + t1 * rayDirection
                - sphere.m_Center;

            potentialHit.Ng_x = geometricNormal.x;
            potentialHit.Ng_y = geometricNormal.y;
            potentialHit.Ng_z = geometricNormal.z;

            ray->tfar = t1;
            *hit = potentialHit;
        }
    }

    void Occluded(const RTCOccludedFunctionNArguments* args)
    {
        // Intersect is only implemented for one sphere per User Geometry
        assert(args->N == 1u);

        bool isRayValid = *(args->valid);

        const EmbreeSphere* spherePtr = static_cast<const EmbreeSphere*>(
            args->geometryUserPtr);
        const EmbreeSphere& sphere = *spherePtr;

        if (!isRayValid) return;

        RTCRay* ray = (RTCRay*)(args->ray);
        glm::vec3 rayOrigin = glm::vec3(ray->org_x, ray->org_y, ray->org_z);
        glm::vec3 rayDirection = glm::vec3(ray->dir_x, ray->dir_y, ray->dir_z);

        const glm::vec3 V = rayOrigin - sphere.m_Center;
        const float A = glm::dot(rayDirection, rayDirection);
        const float B = 2.0f * glm::dot(V, rayDirection);
        const float C = glm::dot(V, V) - (sphere.m_Radius * sphere.m_Radius);

        const float D = B * B - 4.0f * A * C;

        if (D < 0.0f) return;

        const float Q = std::sqrt(D);
        const float rcpA = 1.f / A;
        const float t0 = 0.5f * rcpA * (-B - Q);
        const float t1 = 0.5f * rcpA * (-B + Q);

        RTCHit potentialHit;
        potentialHit.u = 0.0f;
        potentialHit.v = 0.0f;

        // TODO(achal): Set sensible values for `geomID` and `primID` and possibly
        // make use of them during shading.
        potentialHit.geomID = 100u;
        potentialHit.primID = 100u;

        if ((ray->tnear < t0) & (t0 < ray->tfar))
        {
            const glm::vec3 geometricNormal = rayOrigin + t0 * rayDirection
                - sphere.m_Center;

            potentialHit.Ng_x = geometricNormal.x;
            potentialHit.Ng_y = geometricNormal.y;
            potentialHit.Ng_z = geometricNormal.z;

            ray->tfar = -INFINITY;
        }

        if ((ray->tnear < t1) & (t1 < ray->tfar))
        {
            const glm::vec3 geometricNormal = rayOrigin + t1 * rayDirection
                - sphere.m_Center;

            potentialHit.Ng_x = geometricNormal.x;
            potentialHit.Ng_y = geometricNormal.y;
            potentialHit.Ng_z = geometricNormal.z;

            ray->tfar = -INFINITY;
        }
    }

}   // namespace Hikari