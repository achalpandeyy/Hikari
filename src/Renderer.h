#pragma once

#include "Core/Camera.h"
#include "Core/Integrator.h"
#include "Core/Scene.h"

#include <glm/vec2.hpp>

#include <memory>

namespace Hikari
{
    class Renderer
    {
    public:
        Renderer();

        void Render() const;

    private:
        std::shared_ptr<Scene> m_Scene = nullptr;
        std::unique_ptr<Integrator> m_Integrator = nullptr;
        std::shared_ptr<Camera> m_Camera = nullptr;
    };

}   // namespace Hikari