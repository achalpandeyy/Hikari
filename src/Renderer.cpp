#include "Renderer.h"
#include "Ray.h"
#include "Whitted.h"
#include "Sphere.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "BlockGenerator.h"
#include "ImageBlock.h"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"

#include <iostream>
#include <thread>
#include <chrono>

// TODO: Temporarily global
// TODO: Hardcoding the image width and height temporarily - this should
// be something which the user can change at runtime in accordance with
// the resolution enum
unsigned int imageWidth = 640u;
unsigned int imageHeight = 400u;

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
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // Parallel
    glm::vec2 framebufferDimensions(imageWidth, imageHeight);

    BlockGenerator blockGenerator(framebufferDimensions, BLOCK_DIMENSION);
    ImageBlock outputImage(framebufferDimensions);

    std::thread renderThread([&]
    {
        std::cout << "Rendering.." << std::endl;

        tbb::blocked_range<unsigned int> range(0u, blockGenerator.GetBlockCount());

        auto map = [&](const tbb::blocked_range<unsigned int>& range)
        {
            ImageBlock imageBlock(glm::vec2(BLOCK_DIMENSION));

            // !"This `for` loop shouldn't be necessary"!
            // An interesting behaviour: If we remove the `for` loop
            // from here `m_BlocksLeft` rarely hits 0 (which should happen
            // all the time) instead it sometimes stops at 3 sometimes at 1 etc.
            for (unsigned int i = range.begin(); i < range.end(); ++i)
            {
                blockGenerator.NextBlock(imageBlock);
                RenderBlock(imageBlock);
                outputImage.Put(imageBlock);
            }
        };

        tbb::parallel_for(range, map);
        // map(range);
    });

    renderThread.join();
    auto stopTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);

    std::cout << "Render time: " << duration.count() / 1e6 << " seconds." << std::endl;

    outputImage.WriteImage();
}

void Renderer::RenderBlock(ImageBlock& block) const
{
    glm::vec2 rasterCoordinates(block.m_Position.x * BLOCK_DIMENSION, block.m_Position.y * BLOCK_DIMENSION);

    for (unsigned int col = 0; col < block.m_Dimensions.y; ++col)
    {
        for (unsigned int row = 0; row < block.m_Dimensions.x; ++row)
        {
            Ray primaryRay = m_Scene->m_Camera->SpawnRay(glm::vec2(rasterCoordinates.x + row, rasterCoordinates.y + col));

            glm::vec3 color = m_Integrator->Li(primaryRay, *m_Scene, 0);
            unsigned int idx = col * block.m_Dimensions.x + row;
            block.m_Data[idx] = color;
        }
    }
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
        imageWidth,                           // image width
        imageHeight                            // image height
    );
}

}   // namespace Rays