#pragma once

#include "Core/Light.h"
#include "Core/Primitive.h"

#include <embree3/rtcore.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>

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
        RTCRay ToRTCRay(const Ray& ray) const;

        static void DeviceErrorHandler(void* userPtr, const RTCError code, const char* message);

        RTCDevice m_Device = nullptr;
        RTCScene m_Scene = nullptr;
        std::vector< std::shared_ptr<Primitive> > m_Primitives;
    };

}   // namespace Hikari 
