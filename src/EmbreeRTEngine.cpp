#include "EmbreeRTEngine.h"

#include "Interaction.h"
#include "Ray.h"
#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <memory>

namespace Hikari
{

    EmbreeRTEngine::EmbreeRTEngine()
    {
        // TODO(achal): Not make it a `nullptr`
        const char* config = nullptr;
        m_Device = rtcNewDevice(config);
        DeviceErrorHandler(nullptr, rtcGetDeviceError(m_Device), nullptr);
        rtcSetDeviceErrorFunction(m_Device, DeviceErrorHandler, nullptr);

        m_Scene = rtcNewScene(m_Device);
        glm::mat4 obj2Wrld = glm::translate(glm::mat4(1.f), glm::vec3(0.f, -8.5f, 0.f));
        obj2Wrld = glm::rotate(obj2Wrld, glm::radians(15.f), glm::vec3(0.f, 1.f, 0.f));
        obj2Wrld = glm::scale(obj2Wrld, glm::vec3(100.f));
        Transform objToWorld(obj2Wrld);
        AddGeometry("../../models/bunny.obj", objToWorld, glm::vec3(1.f, 0.f, 0.f));
    }

    EmbreeRTEngine::~EmbreeRTEngine()
    {
        if (m_Scene)
            rtcReleaseScene(m_Scene);

        if (m_Device)
            rtcReleaseDevice(m_Device);
    }

    Interaction EmbreeRTEngine::Intersect(const Ray& ray) const
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
        rtcIntersect1(m_Scene, &context, &rayhit);

        if (rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID)
        {
            glm::vec3 normal = glm::normalize(glm::vec3(rayhit.hit.Ng_x, rayhit.hit.Ng_y,
                rayhit.hit.Ng_z));

            Interaction interaction(ray(rayhit.ray.tfar), normal, nullptr);
            interaction.m_Valid = true;
            return interaction;
        }
        else
        {
            return Interaction(false);
        }
    }

    bool EmbreeRTEngine::Occluded(const Ray& ray) const
    {
        RTCIntersectContext context;
        rtcInitIntersectContext(&context);

        RTCRay rtcRay = ToRTCRay(ray);

        rtcOccluded1(m_Scene, &context, &rtcRay);

        return rtcRay.tfar == -INFINITY;
    }

    void EmbreeRTEngine::AddGeometry(
        const char*         path,
        const Transform&    objectToWorld,
        const glm::vec3&    albedo)
    {
        std::unique_ptr<EmbreeGeometry> geom = std::make_unique<EmbreeTriangleMesh>(
            m_Device, path, objectToWorld, albedo);
        geom->Attach(m_Scene);
        rtcCommitScene(m_Scene);
    }

    RTCRay EmbreeRTEngine::ToRTCRay(const Ray& ray) const
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

    void DeviceErrorHandler(void* userPtr, const RTCError code, const char* message)
    {
        if (code == RTC_ERROR_NONE)
        {
            return;
        }

        std::cerr << "Embree: ";
        switch (code)
        {
        case RTC_ERROR_UNKNOWN:
        {
            std::cerr << "RTC_ERROR_UNKNOWN";
            break;
        }
        case RTC_ERROR_INVALID_ARGUMENT:
        {
            std::cerr << "RTC_ERROR_INVALID_ARGUMENT";
            break;
        }
        case RTC_ERROR_INVALID_OPERATION:
        {
            std::cerr << "RTC_ERROR_INVALID_OPERATION";
            break;
        }
        case RTC_ERROR_OUT_OF_MEMORY:
        {
            std::cerr << "RTC_ERROR_OUT_OF_MEMORY";
            break;
        }
        case RTC_ERROR_UNSUPPORTED_CPU:
        {
            std::cerr << "RTC_ERROR_UNSUPPORTED_CPU";
            break;
        }
        case RTC_ERROR_CANCELLED:
        {
            std::cerr << "RTC_ERROR_CANCELLED";
            break;
        }
        default:
        {
            std::cerr << "Invalid Error Code";
            break;
        }
        }

        if (message)
        {
            std::cerr << " (" << message << ")" << std::endl;
        }

        exit(1);
    }

}   // namespace Hikari