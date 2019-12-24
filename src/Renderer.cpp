#include "Renderer.h"
#include "Ray.h"
#include "Whitted.h"
#include "Sphere.h"
#include "PointLight.h"
#include "DirectionalLight.h"

#include "glm/vec3.hpp"

#include <fstream>

namespace Rays
{

Renderer::Renderer()
{
    m_Scene = std::make_unique<Scene>();

    // TODO: Don't hardcode the scene rather read it from file
    CreateScene(*m_Scene);

    m_Integrator = std::make_unique<WhittedIntegrator>();
}

void Renderer::Render() const
{
    // TODO: Hardcoding the image width and height temporarily - this should
    // be something which the user can change at runtime in accordance with
    // the resolution enum
    unsigned int imageWidth = 640u;
    unsigned int imageHeight = 400u;
    for (unsigned int rasterY = 0; rasterY < imageHeight; rasterY++)
    {
        for (unsigned int rasterX = 0; rasterX < imageWidth; rasterX++)
        {
            Ray primaryRay = m_Scene->m_Camera->SpawnRay(rasterX, rasterY);
            glm::vec3 radiance = m_Integrator->Li(primaryRay, *m_Scene, 0);
            m_Scene->m_Camera->SetPixelRadiance(rasterX, rasterY, radiance);
        }
    }
    m_Scene->m_Camera->WriteImage();
}

void Renderer::CreateScene(Scene& scene)
{
    // Objects
    scene.m_Spheres.push_back(std::make_unique<Sphere>
    (
        5.f,                            // radius
        glm::vec3(0.f, 0.f, 0.f),       // center
        glm::vec3(1.f, 0.f, 0.f)        // albedo
    ));
    
    scene.m_Spheres.push_back(std::make_unique<Sphere>
    (
        995.f,                          // radius
        glm::vec3(0.f, -1000.f, 0.f),   // center
        glm::vec3(0.4f, 0.9f, 0.4f)     // albedo
    ));

    // Lights
    scene.m_Lights.push_back(std::make_unique<DirectionalLight>
    (
        glm::vec3(1.f, 1.f, 1.f),
        glm::vec3(1.f),
        3.f
    ));
    
    // Camera
    scene.m_Camera = std::make_unique<Camera>
    (
        glm::vec3(0.f, 0.f, 20.f),      // camera position
        glm::vec3(0.f, 0.f, 0.f),       // look at
        640u,                           // image width
        400u                            // image height
    );
}

}   // namespace Rays