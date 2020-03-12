#pragma once

#include "Core/Light.h"
#include "Core/Primitive.h"

#include <embree3/rtcore.h>

#include <memory>
#include <vector>

namespace Hikari
{
    class Interaction;
    class Ray;

    class Scene
    {
    public:
        Scene(
            const std::vector< std::shared_ptr<Primitive> >&    primitives,
            const std::vector< std::shared_ptr<Light> >&        lights,
            RTCScene                                            rtcScene)
            : m_Primitives(primitives), m_Lights(lights), m_RTCScene(rtcScene)
        {}

        bool Intersect(const Ray& ray, Interaction* interaction) const;

        bool Occluded(const Ray& ray) const;

        std::vector< std::shared_ptr<Light> > m_Lights;

    private:
        RTCRay ToRTCRay(const Ray& ray) const;

        std::vector< std::shared_ptr<Primitive> > m_Primitives;
        RTCScene m_RTCScene;
    };

}   // namespace Hikari 
