#pragma once

#include "Core/Camera.h"
#include "Core/Integrator.h"
#include "Core/Light.h"
#include "Core/Primitive.h"
#include "Core/Scene.h"

#include <embree3/rtcore.h>
#include <glm/glm.hpp>

#include <memory>

namespace Hikari
{
    class Renderer
    {
    public:
        Renderer();

        ~Renderer();

        void Render() const;

        RTCDevice m_RTCDevice;
        RTCScene m_RTCScene;

    private:
        static void DeviceErrorHandler(void* userPtr, const RTCError code, const char* message);

        std::shared_ptr<Scene> m_Scene = nullptr;
        std::unique_ptr<Integrator> m_Integrator = nullptr;
        std::shared_ptr<Camera> m_Camera = nullptr;
    };

    void MakeScene(
        std::vector< std::shared_ptr<Primitive> >&  primitives,
        std::vector< std::shared_ptr<Light> >&      lights,
        RTCDevice                                   device,
        RTCScene                                    scene);

}   // namespace Hikari