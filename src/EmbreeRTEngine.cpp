#include "EmbreeRTEngine.h"

#include "Interaction.h"
#include "Ray.h"
#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <memory>

namespace Hikari
{

    EmbreeRTEngine::EmbreeRTEngine()
    {
        // TODO(achal): Not make it a `nullptr`
        const char* config = nullptr;
        m_Device = rtcNewDevice(config);
        DeviceErrorHandler(nullptr, rtcGetDeviceError(m_Device), nullptr);
        rtcSetDeviceErrorFunction(m_Device, DeviceErrorHandler, nullptr);

        // TODO(achal): Not make the path to scene a `nullptr` and actually
        // parse scene files to make scenes
        const char* path = nullptr;
        m_Scene = std::make_unique<EmbreeScene>(m_Device, path);
    }

    EmbreeRTEngine::~EmbreeRTEngine()
    {
        if (m_Device)
            rtcReleaseDevice(m_Device);
    }

    Interaction EmbreeRTEngine::Intersect(const Ray& ray) const
    {
        return m_Scene->Intersect(ray);
    }

    bool EmbreeRTEngine::Occluded(const Ray& ray) const
    {
        return m_Scene->Occluded(ray);
    }

    void DeviceErrorHandler(void* userPtr, const RTCError code, const char* message)
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