#pragma once

#include "EmbreeScene.h"

#include <embree3/rtcore.h>
#include <glm/glm.hpp>

namespace Hikari
{
    class Interaction;
    class Ray;

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

        const std::vector< std::shared_ptr<Light> >& GetLights() const { return m_Scene->GetLights(); }

    private:
        RTCDevice m_Device = nullptr;
        std::unique_ptr<EmbreeScene> m_Scene = nullptr;
    };

    void DeviceErrorHandler(void* userPtr, const RTCError code, const char* message);

}   // namespace Hikari