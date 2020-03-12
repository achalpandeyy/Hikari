#include "Scene.h"

#include "Core/Interaction.h"
#include "Lights/Diffuse.h"
#include "Lights/Directional.h"
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
        std::shared_ptr<Shape> ajaxMesh = CreateTriangleMeshShape(
            "../../models/ajax.obj",
            glm::vec3(17.f, 0.f, 8.f),
            60.f,
            glm::vec3(0.f, 1.f, 0.f),
            glm::vec3(1.f),
            m_Device,
            m_Scene);

        std::shared_ptr< Texture<glm::vec3> > refGray = std::make_shared< ConstantTexture<glm::vec3> >(
            glm::vec3(0.25f, 0.25f, 0.25f));
        std::shared_ptr< Texture<float> > rougGray = std::make_shared< ConstantTexture<float> >(0.f);
        std::shared_ptr<Material> grayMtl = std::make_shared<MatteMaterial>(refGray, rougGray);

        std::shared_ptr<Primitive> ajaxPrimitive = std::make_shared<Primitive>(ajaxMesh, grayMtl, nullptr);
        m_Primitives.push_back(ajaxPrimitive);

        /*
        // Bunny
        //
        std::shared_ptr<Shape> bunnyMesh = CreateTriangleMeshShape(
            "../../models/bunny.obj",
            glm::vec3(5.f, -2.f, 5.f),
            0.f,
            glm::vec3(0.f, 1.f, 0.f),
            glm::vec3(50.f),
            m_Device,
            m_Scene);
        std::shared_ptr<Primitive> bunnyPrimitive = std::make_shared<Primitive>(bunnyMesh, grayMtl, nullptr);
        m_Primitives.push_back(bunnyPrimitive);
        */

        // Floor
        //
        std::shared_ptr<Shape> floorMesh = CreateTriangleMeshShape(
            "../../models/cornell-box/floor.obj",
            glm::vec3(0.f),
            0.f,
            glm::vec3(0.f, 1.f, 0.f),
            glm::vec3(150.f),
            m_Device,
            m_Scene
        );

        std::shared_ptr< Texture<glm::vec3> > ref = std::make_shared< ConstantTexture<glm::vec3> >(
            glm::vec3(0.73f, 0.73f, 0.73f));
        std::shared_ptr< Texture<float> > roug = std::make_shared< ConstantTexture<float> >(0.f);
        std::shared_ptr<Material> whiteMtl = std::make_shared<MatteMaterial>(ref, roug);

        std::shared_ptr<Primitive> floor = std::make_shared<Primitive>(floorMesh, whiteMtl, nullptr);
        m_Primitives.push_back(floor);

        // White Area Light
        //
        std::shared_ptr<Shape> whitelightSphere = std::make_shared<Sphere>(glm::vec3(-40.f, 48.f, 44.f), 25.f, m_Device, m_Scene);

        std::shared_ptr<AreaLight> whiteDal = std::make_shared<DiffuseAreaLight>(1u, glm::vec3(4.f, 4.f, 4.f), whitelightSphere, false);
        m_Lights.push_back(whiteDal);

        std::shared_ptr<Primitive> whiteLight = std::make_shared<Primitive>(whitelightSphere, whiteMtl, whiteDal);
        m_Primitives.push_back(whiteLight);

        /*
        // Ceiling
        //
        std::shared_ptr<Shape> ceilingMesh = CreateTriangleMeshShape(
            "../../models/cornell-box/ceiling.obj",
            glm::vec3(0.f),
            0.f,
            glm::vec3(0.f, 0.f, -1.f),
            glm::vec3(10.f),
            m_Device,
            m_Scene
        );
        std::shared_ptr<Primitive> ceiling = std::make_shared<Primitive>(ceilingMesh, whiteMtl, nullptr);
        m_Primitives.push_back(ceiling);
        */

        // Backwall
        //
        std::shared_ptr<Shape> backwallMesh = CreateTriangleMeshShape(
            "../../models/cornell-box/backwall.obj",
            glm::vec3(0.f),
            0.f,
            glm::vec3(0.f, 1.f, 0.f),
            glm::vec3(150.f),
            m_Device,
            m_Scene
        );
        std::shared_ptr<Primitive> backwall = std::make_shared<Primitive>(backwallMesh, whiteMtl, nullptr);
        m_Primitives.push_back(backwall);

        /*
        // Rightwall
        //
        std::shared_ptr<Shape> rightwallMesh = CreateTriangleMeshShape(
            "../../models/cornell-box/rightwall.obj",
            glm::vec3(0.f),
            0.f,
            glm::vec3(0.f, 1.f, 0.f),
            glm::vec3(10.f),
            m_Device,
            m_Scene
        );

        std::shared_ptr< Texture<glm::vec3> > refRed = std::make_shared< ConstantTexture<glm::vec3> >(
            glm::vec3(0.65f, 0.05f, 0.05f));
        std::shared_ptr< Texture<float> > rougRed = std::make_shared< ConstantTexture<float> >(0.f);
        std::shared_ptr<Material> redMtl = std::make_shared<MatteMaterial>(refRed, rougRed);

        std::shared_ptr<Primitive> rightwall = std::make_shared<Primitive>(rightwallMesh, redMtl, nullptr);
        m_Primitives.push_back(rightwall);

        // Leftwall
        //
        std::shared_ptr<Shape> leftwallMesh = CreateTriangleMeshShape(
            "../../models/cornell-box/leftwall.obj",
            glm::vec3(0.f),
            0.f,
            glm::vec3(0.f, 1.f, 0.f),
            glm::vec3(10.f),
            m_Device,
            m_Scene
        );
        std::shared_ptr< Texture<glm::vec3> > refGreen = std::make_shared< ConstantTexture<glm::vec3> >(
            glm::vec3(0.12f, 0.45f, 0.12f));
        std::shared_ptr< Texture<float> > rougGreen = std::make_shared< ConstantTexture<float> >(0.f);
        std::shared_ptr<Material> greenMtl = std::make_shared<MatteMaterial>(refGreen, rougGreen);

        std::shared_ptr<Primitive> leftwall = std::make_shared<Primitive>(leftwallMesh, greenMtl, nullptr);
        m_Primitives.push_back(leftwall);
        */

        /*
        // White Area Light
        //
        std::shared_ptr<Shape> whitelightSphere = std::make_shared<Sphere>(glm::vec3(7.5f, 17.5f, 7.5f), 2.f, m_Device, m_Scene);

        std::shared_ptr<AreaLight> whiteDal = std::make_shared<DiffuseAreaLight>(1u, glm::vec3(3.f, 3.f, 3.f), whitelightSphere, false);
        m_Lights.push_back(whiteDal);

        std::shared_ptr<Primitive> whiteLight = std::make_shared<Primitive>(whitelightSphere, whiteMtl, whiteDal);
        m_Primitives.push_back(whiteLight);

        // White Area Light
        //
        std::shared_ptr<Shape> greenlightSphere = std::make_shared<Sphere>(glm::vec3(7.5f, 17.5f, -7.5f), 2.f, m_Device, m_Scene);

        std::shared_ptr<AreaLight> greenDal = std::make_shared<DiffuseAreaLight>(1u, glm::vec3(3.f, 3.f, 3.f), greenlightSphere, false);
        m_Lights.push_back(greenDal);

        std::shared_ptr<Primitive> greenLight = std::make_shared<Primitive>(greenlightSphere, whiteMtl, greenDal);
        m_Primitives.push_back(greenLight);

        // White Area Light
        //
        std::shared_ptr<Shape> blueLightSphere = std::make_shared<Sphere>(glm::vec3(-7.5f, 17.5f, 7.5f), 2.f, m_Device, m_Scene);

        std::shared_ptr<AreaLight> blueDal = std::make_shared<DiffuseAreaLight>(1u, glm::vec3(3.f, 3.f, 3.f), blueLightSphere, false);
        m_Lights.push_back(blueDal);

        std::shared_ptr<Primitive> blueLight = std::make_shared<Primitive>(blueLightSphere, whiteMtl, blueDal);
        m_Primitives.push_back(blueLight);

        // White Area Light
        //
        std::shared_ptr<Shape> redLightSphere = std::make_shared<Sphere>(glm::vec3(-7.5f, 17.5f, -7.5f), 2.f, m_Device, m_Scene);

        std::shared_ptr<AreaLight> redDal = std::make_shared<DiffuseAreaLight>(1u, glm::vec3(3.f, 3.f, 3.f), redLightSphere, false);
        m_Lights.push_back(redDal);

        std::shared_ptr<Primitive> redLight = std::make_shared<Primitive>(redLightSphere, whiteMtl, redDal);
        m_Primitives.push_back(redLight);
        */

        /*
        // Greenish Analytic Sphere for the ground
        //
        std::shared_ptr<Shape> greenSphere = std::make_shared<Sphere>(glm::vec3(0.f, -1000.f, 0.f), 995.f, m_Device, m_Scene);

        std::shared_ptr< Texture<glm::vec3> > reflectivity2 = std::make_shared< ConstantTexture<glm::vec3> >(
            glm::vec3(0.4f, 0.9f, 0.4f));
        std::shared_ptr< Texture<float> > roughness2 = std::make_shared< ConstantTexture<float> >(90.f);
        std::shared_ptr<Material> greenMtl = std::make_shared<MatteMaterial>(reflectivity2, roughness2);

        std::shared_ptr<Primitive> greenSpherePrimitive = std::make_shared<Primitive>(greenSphere, greenMtl, nullptr);
        m_Primitives.push_back(greenSpherePrimitive);
        */

        /*
        // White Area Light
        //
        std::shared_ptr<Shape> whitelightSphere = std::make_shared<Sphere>(glm::vec3(7.f, 10.f, -7.f), 2.f, m_Device, m_Scene);

        std::shared_ptr<AreaLight> whiteDal = std::make_shared<DiffuseAreaLight>(1u, glm::vec3(2.5f, 2.5f, 2.5f), whitelightSphere, false);
        m_Lights.push_back(whiteDal);

        std::shared_ptr<Primitive> whiteLight = std::make_shared<Primitive>(whitelightSphere, whiteMtl, whiteDal);
        m_Primitives.push_back(whiteLight);

        // Green Area Light
        //
        std::shared_ptr<Shape> greenlightSphere = std::make_shared<Sphere>(glm::vec3(7.f, 17.f, 7.f), 2.f, m_Device, m_Scene);

        std::shared_ptr<AreaLight> greenDal = std::make_shared<DiffuseAreaLight>(1u, glm::vec3(0.5f, 2.5f, 0.5f), greenlightSphere, false);
        m_Lights.push_back(greenDal);

        std::shared_ptr<Primitive> greenLight = std::make_shared<Primitive>(greenlightSphere, whiteMtl, greenDal);
        m_Primitives.push_back(greenLight);

        // Blue Area Light
        //
        std::shared_ptr<Shape> blueLightSphere = std::make_shared<Sphere>(glm::vec3(-7.f, 3.f, 5.f), 2.f, m_Device, m_Scene);

        std::shared_ptr<AreaLight> blueDal = std::make_shared<DiffuseAreaLight>(1u, glm::vec3(0.75f, 0.75f, 2.5f), blueLightSphere, false);
        m_Lights.push_back(blueDal);

        std::shared_ptr<Primitive> blueLight = std::make_shared<Primitive>(blueLightSphere, whiteMtl, blueDal);
        m_Primitives.push_back(blueLight);

        // Red Area Light
        //
        std::shared_ptr<Shape> redLightSphere = std::make_shared<Sphere>(glm::vec3(-7.f, 17.f, 0.f), 2.f, m_Device, m_Scene);

        std::shared_ptr<AreaLight> redDal = std::make_shared<DiffuseAreaLight>(1u, glm::vec3(2.5f, 0.5f, 0.5f), redLightSphere, false);
        m_Lights.push_back(redDal);

        std::shared_ptr<Primitive> redLight = std::make_shared<Primitive>(redLightSphere, whiteMtl, redDal);
        m_Primitives.push_back(redLight);
        */

        rtcCommitScene(m_Scene);
    }

    Scene::~Scene()
    {
        if (m_Scene)
            rtcReleaseScene(m_Scene);

        if (m_Device)
            rtcReleaseDevice(m_Device);
    }

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
        rtcIntersect1(m_Scene, &context, &rayhit);

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