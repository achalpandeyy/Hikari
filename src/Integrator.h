#pragma once

#include "EmbreeRTEngine.h"

#include <glm/glm.hpp>

#include <memory>

namespace Hikari
{

    class Ray;

    class Integrator
    {
    public:
        Integrator(std::shared_ptr<EmbreeRTEngine> rtEngine) : m_RTEngine(rtEngine) {}
        virtual ~Integrator() {}

        virtual glm::vec3 Li(const Ray& ray) const = 0;

    protected:
        std::shared_ptr<EmbreeRTEngine> m_RTEngine;
    };

}   // namespace Rays