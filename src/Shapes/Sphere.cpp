#include "Sphere.h"

#include "Core/Interaction.h"
#include "Math/Constants.h"
#include "Math/Utility.h"

#include <algorithm>

namespace Hikari
{
    // TODO(achal): Put it somewhere else.
    glm::vec3 UniformSampleSphere(const glm::vec2& e)
    {
        float y = 1.f - 2.f * e[0];
        float sinTheta = std::sqrt(std::max(0.f, 1.f - y * y));
        float phi = 2.f * PI * e[1];
        return glm::vec3(sinTheta * std::cos(phi), y, sinTheta * std::sin(phi));
    }

    Sphere::Sphere(
        const glm::vec3&    center,
        float               radius,
        RTCDevice           device,
        RTCScene            scene)
        : Shape(rtcNewGeometry(device, RTC_GEOMETRY_TYPE_USER)), m_Center(center), m_Radius(radius)
    {
        // Set all callbacks
        rtcSetGeometryUserPrimitiveCount(m_Geometry, 1u);
        rtcSetGeometryUserData(m_Geometry, this);

        rtcSetGeometryBoundsFunction(m_Geometry, Bounds, nullptr);
        rtcSetGeometryIntersectFunction(m_Geometry, Intersect);
        rtcSetGeometryOccludedFunction(m_Geometry, Occluded);

        rtcCommitGeometry(m_Geometry);

        m_Id = rtcAttachGeometry(scene, m_Geometry);
    }

    Sphere::~Sphere()
    {
        if (m_Geometry)
            rtcReleaseGeometry(m_Geometry);
    }

    float Sphere::SurfaceArea() const
    {
        return 4.f * PI * m_Radius * m_Radius;
    }

    Interaction Sphere::AreaSample(const glm::vec2& sample, float* pdf) const
    {
        Interaction interaction;
        const glm::vec3& pSphere = UniformSampleSphere(sample);
        interaction.m_Position = m_Radius * pSphere + m_Center;
        interaction.m_Normal = pSphere;

        *pdf = 1.f / SurfaceArea();

        return interaction;
    }

    float Sphere::Pdf(const Interaction& pShape, const glm::vec3& wi) const
    {
        return 1.f / SurfaceArea();
    }

    Interaction Sphere::SolidAngleSample(const Interaction& illumPoint, const glm::vec2& sample, float* pdf) const
    {
        float dc = glm::distance(m_Center, illumPoint.m_Position);
        float invDc = 1.f / dc;

        float sinThetaMax = m_Radius * invDc;
        float sinThetaMaxSq = sinThetaMax * sinThetaMax;
        float invSinThetaMax = 1.f / sinThetaMax;

        float cosThetaMax = glm::sqrt(glm::max(0.f, 1.f - sinThetaMaxSq));
        float cosTheta = sample[0] * (cosThetaMax - 1.f) + 1.f;
        float sinThetaSq = 1.f - cosTheta * cosTheta;

        float cosAlpha = sinThetaSq * invSinThetaMax +
            cosTheta * glm::sqrt(glm::max(0.f, 1.f - sinThetaSq * invSinThetaMax * invSinThetaMax));
        float sinAlpha = glm::sqrt(glm::max(0.f, 1.f - cosAlpha * cosAlpha));
        float phi = 2.f * PI * sample[1];

        glm::vec3 localY = (m_Center - illumPoint.m_Position) / dc;
        glm::vec3 localX, localZ;
        OrthonormalBasis(localY, localX, localZ);

        Interaction interaction;
        interaction.m_Normal = glm::normalize((-localX) * sinAlpha * glm::cos(phi) + (-localY) * cosAlpha +
            (-localZ) * sinAlpha * glm::sin(phi));
        interaction.m_Position = m_Center + m_Radius * interaction.m_Normal;

        *pdf = 1.f / (2.f * PI * (1.f - cosThetaMax));

        return interaction;
    }

    void Sphere::Bounds(const RTCBoundsFunctionArguments* args)
    {
        const Sphere* spherePtr = static_cast<const Sphere*>(
            args->geometryUserPtr);
        const Sphere& sphere = *spherePtr;

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
    // https://github.com/embree/embree/blob/master/tutorials/user_geometry/user_geometry_device.cpp#L340
    // Many features which are included above are stripped down from here.
    // Refer to the above link for more features to specialize the implementation.
    //
    void Sphere::Intersect(const RTCIntersectFunctionNArguments* args)
    {
        // Intersect is only implemented for one sphere per User Geometry
        assert(args->N == 1u);

        bool isRayValid = *(args->valid);
        const Sphere* spherePtr = static_cast<const Sphere*>(
            args->geometryUserPtr);
        const Sphere& sphere = *spherePtr;

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

        // TODO(achal): Set sensible values for `primID` and possibly
        // make use of them during shading.
        potentialHit.geomID = sphere.m_Id;
        potentialHit.primID = 100u;

        if ((ray->tnear < t0) & (t0 < ray->tfar))
        {
            const glm::vec3 geometricNormal = rayOrigin + t0 * rayDirection - sphere.m_Center;

            potentialHit.Ng_x = geometricNormal.x;
            potentialHit.Ng_y = geometricNormal.y;
            potentialHit.Ng_z = geometricNormal.z;

            potentialHit.v = std::acosf(geometricNormal.y / sphere.m_Radius) * INVPI;
            potentialHit.u = (std::atan2(geometricNormal.z, geometricNormal.x) + PI) * 0.5f * INVPI;

            ray->tfar = t0;
            *hit = potentialHit;
        }

        if ((ray->tnear < t1) & (t1 < ray->tfar))
        {
            const glm::vec3 geometricNormal = rayOrigin + t1 * rayDirection - sphere.m_Center;

            potentialHit.Ng_x = geometricNormal.x;
            potentialHit.Ng_y = geometricNormal.y;
            potentialHit.Ng_z = geometricNormal.z;

            potentialHit.v = std::acosf(geometricNormal.y / sphere.m_Radius) * INVPI;
            potentialHit.u = (std::atan2(geometricNormal.z, geometricNormal.x) + PI) * 0.5f * INVPI;

            ray->tfar = t1;
            *hit = potentialHit;
        }
    }

    void Sphere::Occluded(const RTCOccludedFunctionNArguments* args)
    {
        // Intersect is only implemented for one sphere per User Geometry
        assert(args->N == 1u);

        bool isRayValid = *(args->valid);

        const Sphere* spherePtr = static_cast<const Sphere*>(
            args->geometryUserPtr);
        const Sphere& sphere = *spherePtr;

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