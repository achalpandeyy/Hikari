#include "Scene.h"

#include "Core/Interaction.h"
#include "Lights/Diffuse.h"
#include "Lights/Point.h"
#include "Lights/Spot.h"
#include "Materials/Matte.h"
#include "Math/Ray.h"
#include "Math/Transform.h"
#include "Shapes/Sphere.h"
#include "Shapes/TriangleMesh.h"
#include "Textures/Constant.h"

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

        // Ajax
        //
        Transform meshToWorld2 = Translate(glm::vec3(6.0f, -8.f, 2.f));
        meshToWorld2 = meshToWorld2 * Rotate(60.f, glm::vec3(0.f, 1.f, 0.f));
        meshToWorld2 = meshToWorld2 * Scale(glm::vec3(0.5f));

        std::shared_ptr<Shape> ajaxMesh = std::make_shared<TriangleMesh>(m_Device, m_Scene, meshToWorld2, "../../models/ajax.obj");

        std::shared_ptr< Texture<glm::vec3> > ref = std::make_shared< ConstantTexture<glm::vec3> >(
            glm::vec3(0.5f, 0.5f, 0.5f));
        std::shared_ptr< Texture<float> > roug = std::make_shared< ConstantTexture<float> >(90.f);
        std::shared_ptr<Material> grayMtl = std::make_shared<MatteMaterial>(ref, roug);

        std::shared_ptr<Primitive> ajaxPrimitive = std::make_shared<Primitive>(ajaxMesh, grayMtl, nullptr);
        m_Primitives.push_back(ajaxPrimitive);


        // Red Analytic Sphere
        //
        /*
        Transform redSphereToWorld = Scale(glm::vec3(5.f));

        std::shared_ptr<Shape> redSphere = std::make_shared<Sphere>(m_Device, m_Scene, redSphereToWorld);

        std::shared_ptr< Texture<glm::vec3> > reflectivity1 = std::make_shared< ConstantTexture<glm::vec3> >(
            glm::vec3(1.f, 0.f, 0.f));
        std::shared_ptr< Texture<float> > roughness1 = std::make_shared< ConstantTexture<float> >(90.f);
        std::shared_ptr<Material> redMtl = std::make_shared<MatteMaterial>(reflectivity1, roughness1);

        std::shared_ptr<Primitive> redSpherePrimitive = std::make_shared<Primitive>(redSphere, redMtl, nullptr);
        m_Primitives.push_back(redSpherePrimitive);
        */

        // Greenish Analytic Sphere for the ground
        //
        Transform greenSphereToWorld = Translate(glm::vec3(0.f, -1000.f, 0.f));
        greenSphereToWorld = greenSphereToWorld * Scale(glm::vec3(995.f));

        std::shared_ptr<Shape> greenSphere = std::make_shared<Sphere>(m_Device, m_Scene, greenSphereToWorld);

        std::shared_ptr< Texture<glm::vec3> > reflectivity2 = std::make_shared< ConstantTexture<glm::vec3> >(
            glm::vec3(0.4f, 0.9f, 0.4f));
        std::shared_ptr< Texture<float> > roughness2 = std::make_shared< ConstantTexture<float> >(90.f);
        std::shared_ptr<Material> greenMtl = std::make_shared<MatteMaterial>(reflectivity2, roughness2);

        std::shared_ptr<Primitive> greenSpherePrimitive = std::make_shared<Primitive>(greenSphere, greenMtl, nullptr);
        m_Primitives.push_back(greenSpherePrimitive);


        // Area light sphere
        //
        Transform lightSphereToWorld = Translate(glm::vec3(35.f, 25.f, 0.f));
        lightSphereToWorld = lightSphereToWorld * Scale(glm::vec3(1.f));

        std::shared_ptr<Shape> lightSphere = std::make_shared<Sphere>(m_Device, m_Scene, lightSphereToWorld);

        std::shared_ptr< Texture<glm::vec3> > lightRefl = std::make_shared< ConstantTexture<glm::vec3> >(
            glm::vec3(1.f));
        std::shared_ptr< Texture<float> > lightRoug = std::make_shared< ConstantTexture<float> >(0.f);
        std::shared_ptr<Material> whiteMtl = std::make_shared<MatteMaterial>(lightRefl, lightRoug);

        std::shared_ptr<AreaLight> dal = std::make_shared<DiffuseAreaLight>(1u, glm::vec3(1.5f), lightSphere, false);
        m_Lights.push_back(dal);

        std::shared_ptr<Primitive> lightSpherePrimitive = std::make_shared<Primitive>(lightSphere, whiteMtl, dal);
        m_Primitives.push_back(lightSpherePrimitive);

        rtcCommitScene(m_Scene);

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
            assert(m_Primitives.size() > rayhit.hit.geomID);

            return Interaction(
                ray(rayhit.ray.tfar),
                glm::normalize(glm::vec3(rayhit.hit.Ng_x, rayhit.hit.Ng_y, rayhit.hit.Ng_z)),
                glm::vec2(rayhit.hit.u, rayhit.hit.v),
                m_Primitives[rayhit.hit.geomID].get());
        }
        return Interaction(glm::vec3(0.f), glm::vec3(0.f), glm::vec2(0.f), nullptr);
    }

    bool Scene::Occluded(const Ray& ray) const
    {
        RTCIntersectContext context;
        rtcInitIntersectContext(&context);

        RTCRay rtcRay = ToRTCRay(ray);

        rtcOccluded1(m_Scene, &context, &rtcRay);

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