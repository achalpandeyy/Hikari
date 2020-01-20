#pragma once

#include "EmbreeGeometry.h"

#include <embree3/rtcore.h>
#include <glm/glm.hpp>

namespace Hikari
{
    class Interaction;
    class Ray;
    class Transform;

    class EmbreeRTEngine
    {
    public:
        // TODO(achal): Pass scene file to the RTEngine to construct its
        // version of the Scene, or pass in a generic Hikari::Scene object
        // which can be easily converted to different RTEngines' scene representation
        EmbreeRTEngine();
        ~EmbreeRTEngine();

        Interaction Intersect(const Ray& ray) const;
        bool Occluded(const Ray& ray) const;

    private:
        void AddGeometry(
            const char*         path,
            const Transform&    objectToWorld,
            const glm::vec3&    albedo);

        RTCRay ToRTCRay(const Ray& ray) const;

        RTCDevice m_Device;
        RTCScene m_Scene;
    };

    void DeviceErrorHandler(void* userPtr, const RTCError code, const char* message);

}   // namespace Hikari