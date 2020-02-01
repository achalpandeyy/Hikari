#include "Scene.h"

#include "DirectionalLight.h"
#include "Interaction.h"
#include "Ray.h"
#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace Hikari
{

    Scene::Scene(const char* path)
    {
        // TODO(achal): Not make it a `nullptr`
        const char* config = nullptr;
        m_Device = rtcNewDevice(config);
        DeviceErrorHandler(nullptr, rtcGetDeviceError(m_Device), nullptr);
        rtcSetDeviceErrorFunction(m_Device, DeviceErrorHandler, nullptr);

        // Hard code scene here for now
        m_Scene = rtcNewScene(m_Device);

        // Add a triangle mesh
        // glm::vec3 meshAlbedo = glm::vec3(1.f, 1.f, 0.f);
        // Transform meshToWorld = Translate(glm::vec3(0.f, -8.5f, 0.f));
        // meshToWorld = meshToWorld * Rotate(15.f, glm::vec3(0.f, 1.f, 0.f));
        // meshToWorld = meshToWorld * Scale(glm::vec3(100.f));
        // AddTriangleMesh("../../models/bunny.obj", meshToWorld, meshAlbedo);

        glm::vec3 redSphereAlbedo(1.f, 0.f, 0.f);
        Transform redSphereToWorld = Scale(glm::vec3(5.f));
        AddSphere(redSphereToWorld, redSphereAlbedo, glm::vec3(0.f));

        // Add an analytic sphere
        glm::vec3 sphereAlbedo(0.4f, 0.9f, 0.4f);
        Transform sphereToWorld = Translate(glm::vec3(0.f, -1000.f, 0.f));
        sphereToWorld = sphereToWorld * Scale(glm::vec3(995.f));
        AddSphere(sphereToWorld, sphereAlbedo, glm::vec3(0.f));

        glm::vec3 lightAlbedo(0.f);
        Transform lightToWorld = Scale(glm::vec3(5.f)); 
        lightToWorld = lightToWorld * Translate(glm::vec3(10.f, 10.f, 0.f));
        AddSphere(lightToWorld, lightAlbedo, glm::vec3(15.f));

        rtcCommitScene(m_Scene);

        // Lights
        m_Lights.push_back(std::make_shared<DirectionalLight>
        (
            glm::vec3(1.f, 1.f, 1.f),
            glm::vec3(1.f),
            3.f
        ));
    }

    Scene::~Scene()
    {
        if (m_Scene)
            rtcReleaseScene(m_Scene);

        if (m_Device)
            rtcReleaseDevice(m_Device);
    }

    Interaction Scene::Intersect(const Ray& ray) const
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
            assert(m_Shapes.size() > rayhit.hit.geomID);

            return Interaction(
                ray(rayhit.ray.tfar),
                glm::normalize(glm::vec3(rayhit.hit.Ng_x, rayhit.hit.Ng_y,
                rayhit.hit.Ng_z)),
                m_Shapes[rayhit.hit.geomID].get());
        }
        return Interaction(glm::vec3(0.f), glm::vec3(0.f), nullptr);
    }

    bool Scene::Occluded(const Ray& ray) const
    {
        RTCIntersectContext context;
        rtcInitIntersectContext(&context);

        RTCRay rtcRay = ToRTCRay(ray);

        rtcOccluded1(m_Scene, &context, &rtcRay);

        return rtcRay.tfar == -INFINITY;
    }

    void Scene::AddTriangleMesh(
        const char*         path,
        const Transform&    objectToWorld,
        const glm::vec3&    albedo,
        const glm::vec3&    emission)
    {
        std::shared_ptr<Shape> geom = std::make_shared<TriangleMesh>(m_Device,
            objectToWorld, path, albedo, emission);
        geom->Attach(m_Scene);
        m_Shapes.push_back(geom);
    }

    void Scene::AddSphere(
        const Transform&    objectToWorld,
        const glm::vec3&    albedo,
        const glm::vec3&    emission)
    {
        std::shared_ptr<Shape> geom = std::make_shared<Sphere>(
            m_Device, objectToWorld, albedo, emission);
        geom->Attach(m_Scene);
        m_Shapes.push_back(geom);
    }

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

    void Scene::DeviceErrorHandler(void* userPtr, const RTCError code, const char* message)
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