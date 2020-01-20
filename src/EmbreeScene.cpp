#include "EmbreeScene.h"

#include "DirectionalLight.h"
#include "Interaction.h"
#include "Ray.h"
#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hikari
{

    EmbreeScene::EmbreeScene(RTCDevice device, const char* path)
        : m_Device(device)
    {
        // Hard code scene here for now
        m_Scene = rtcNewScene(m_Device);

        // Add a triangle mesh
        glm::mat4 obj2Wrld = glm::translate(glm::mat4(1.f), glm::vec3(0.f, -8.5f, 0.f));
        obj2Wrld = glm::rotate(obj2Wrld, glm::radians(15.f), glm::vec3(0.f, 1.f, 0.f));
        obj2Wrld = glm::scale(obj2Wrld, glm::vec3(100.f));
        Transform objToWorld(obj2Wrld);
        AddTriangleMesh("../../models/bunny.obj", objToWorld, glm::vec3(1.f, 1.f, 0.f));

        // Add an analytic sphere
        glm::vec3 center(0.f, -1000.f, 0.f);
        float radius = 995.f;
        glm::vec3 albedo(0.4f, 0.9f, 0.4f);
        AddSphere(center, radius, albedo);

        rtcCommitScene(m_Scene);

        // Lights
        m_Lights.push_back(std::make_shared<DirectionalLight>
        (
            glm::vec3(1.f, 1.f, 1.f),
            glm::vec3(1.f),
            3.f
        ));
    }

    EmbreeScene::~EmbreeScene()
    {
        if (m_Device)
            rtcReleaseDevice(m_Device);

        if (m_Scene)
            rtcReleaseScene(m_Scene);
    }

    Interaction EmbreeScene::Intersect(const Ray& ray) const
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

        Interaction interaction;
        if (rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID)
        {
            interaction.m_HitPoint = ray(rayhit.ray.tfar);
            interaction.m_Normal = glm::normalize(glm::vec3(rayhit.hit.Ng_x, rayhit.hit.Ng_y,
                rayhit.hit.Ng_z));
            interaction.m_Albedo = m_Albedos.at(rayhit.hit.geomID);
            interaction.m_Valid = true;
        }
        else
        {
            interaction.m_Valid = false;
        }
        return interaction;
    }

    bool EmbreeScene::Occluded(const Ray& ray) const
    {
        RTCIntersectContext context;
        rtcInitIntersectContext(&context);

        RTCRay rtcRay = ToRTCRay(ray);

        rtcOccluded1(m_Scene, &context, &rtcRay);

        return rtcRay.tfar == -INFINITY;
    }

    void EmbreeScene::AddTriangleMesh(
        const char*         path,
        const Transform&    objectToWorld,
        const glm::vec3&    albedo)
    {
        std::shared_ptr<EmbreeGeometry> geom = std::make_shared<EmbreeTriangleMesh>(m_Device, path,
            objectToWorld, albedo);
        unsigned int geomId = geom->Attach(m_Scene);
        geom->SetId(geomId);
        m_Albedos[geomId] = albedo;
        m_Geometries.push_back(geom);
    }

    void EmbreeScene::AddSphere(
        const glm::vec3&    center,
        float               radius,
        const glm::vec3&    albedo)
    {
        std::shared_ptr<EmbreeGeometry> geom = std::make_shared<EmbreeSphere>(
            m_Device, center, radius, albedo);
        unsigned int geomId = geom->Attach(m_Scene);
        geom->SetId(geomId);
        m_Albedos[geomId] = albedo;
        m_Geometries.push_back(geom);
    }

    RTCRay EmbreeScene::ToRTCRay(const Ray& ray) const
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