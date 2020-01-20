#pragma once

#include "EmbreeGeometry.h"
#include "Light.h"

#include <embree3/rtcore.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>
#include <unordered_map>

namespace Hikari
{
    class Interaction;
    class Ray;
    class Transform;

    class EmbreeScene
    {
    public:
        EmbreeScene(RTCDevice device, const char* path);
        ~EmbreeScene();

        Interaction Intersect(const Ray& ray) const;
        bool Occluded(const Ray& ray) const;

        const std::vector< std::shared_ptr<Light> >& GetLights() const { return m_Lights; }

    private:
        void AddTriangleMesh(
            const char* path,
            const Transform& objectToWorld,
            const glm::vec3& albedo);

        void AddSphere(
            const glm::vec3&    center,
            float               radius,
            const glm::vec3&    albedo);

        RTCRay ToRTCRay(const Ray& ray) const;

        RTCDevice m_Device = nullptr;
        RTCScene m_Scene = nullptr;
        std::vector< std::shared_ptr<EmbreeGeometry> > m_Geometries;
        std::vector< std::shared_ptr<Light> > m_Lights;
        std::unordered_map<unsigned int, glm::vec3> m_Albedos;
    };

}   // namespace Hikari 
