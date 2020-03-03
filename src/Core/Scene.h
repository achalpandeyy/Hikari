#pragma once

#include "Shape.h"
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

    class Scene
    {
    public:
        Scene(const char* path);
        ~Scene();

        Interaction Intersect(const Ray& ray) const;
        bool Occluded(const Ray& ray) const;

        std::vector< std::shared_ptr<Light> > m_Lights;

    private:
        void AddTriangleMesh(
            const char*                         path,
            const Transform&                    objectToWorld,
            const std::shared_ptr<Material>&    material,
            const glm::vec3&                    emission);

        void AddSphere(
            const Transform&                    ojbectToWorld,
            const std::shared_ptr<Material>&    material,
            const glm::vec3&                    emission);

        RTCRay ToRTCRay(const Ray& ray) const;

        static void DeviceErrorHandler(void* userPtr, const RTCError code, const char* message);

        RTCDevice m_Device = nullptr;
        RTCScene m_Scene = nullptr;
        std::vector< std::shared_ptr<Shape> > m_Shapes;
    };

}   // namespace Hikari 