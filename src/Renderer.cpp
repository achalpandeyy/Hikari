#include "Renderer.h"

#include "BlockGenerator.h"
#include "ImageBlock.h"
#include "Integrators/DirectLighting.h"
#include "Integrators/Path.h"
#include "Integrators/Whitted.h"
#include "Math/Ray.h"
#include "Core/Sampler.h"
#include "Math/Transform.h"

#include <glm/glm.hpp>

namespace Hikari
{
    const glm::ivec2 resolution(1024, 1024);

    Renderer::Renderer()
    {
        // TODO(achal): Not make the path to scene a `nullptr` and actually
        // parse scene files to make scenes
        const char* path = nullptr;
        m_Scene = std::make_shared<Scene>(path);

        m_Camera = std::make_shared<Camera>
        (
            glm::vec3(0.f, 10.f, 35.f),      // camera position
            glm::vec3(0.f, 10.f, 0.f),       // look at
            glm::vec3(0.f, 1.f, 0.f),       // up
            resolution
        );

        const unsigned int maxDepth = 5u;
        // m_Integrator = std::make_unique<DirectLightingIntegrator>(LightStrategy::UniformSampleAll, maxDepth, m_Camera);
        m_Integrator = std::make_unique<PathIntegrator>(maxDepth, m_Camera);
        // m_Integrator = std::make_unique<WhittedIntegrator>(maxDepth, m_Camera);
    }

    void Renderer::Render() const
    {
        m_Integrator->Render(*m_Scene);
    }

}   // namespace Hikari