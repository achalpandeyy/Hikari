#include "Renderer.h"

#include "Ray.h"
#include "Whitted.h"
#include "Sphere.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "BlockGenerator.h"
#include "ImageBlock.h"
#include "Transform.h"
#include "Triangle.h"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"

#include <chrono>
#include <iostream>
#include <thread>


namespace Hikari
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

    glm::vec2 framebufferDimensions(GetResolution().x, GetResolution().y);

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
        
        if (m_RenderOptions.m_Multithreading)
        {
            std::cout << "Parallel" << std::endl;
            tbb::parallel_for(range, map);
        }
        else
        {
            std::cout << "Sequential" << std::endl;
            map(range);
        }
    });

    renderThread.join();
    auto stopTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);

    std::cout << "Rendered!" << std::endl;
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

void Renderer::SetResolution(const E_Resolution& res)
{
    m_RenderOptions.m_Resolution = res;
    std::cout << "Resolution: ";
    switch (res)
    {
    case _640x400_:
        std::cout << "640 x 400" << std::endl;
        m_Scene->m_Camera->SetResolution(glm::vec2(640, 400));
        break;
    case _320x200_:
        std::cout << "320 x 200" << std::endl;
        m_Scene->m_Camera->SetResolution(glm::vec2(320, 200));
        break;
    case _1x1_:
        std::cout << "1 x 1" << std::endl;
        m_Scene->m_Camera->SetResolution(glm::vec2(1, 1));
        break;
    }
}

glm::vec2 Renderer::GetResolution() const
{
    return m_Scene->m_Camera->GetResolution();
}

void Renderer::CreateScene(Scene& scene)
{
    // Objects
    /*
    glm::mat4 spObj2Wrld = glm::translate(glm::mat4(1.f), glm::vec3(7.f, 0.f, 0.f));
    scene.m_Shapes.push_back(std::make_unique<Sphere>
    (
        new Transform(spObj2Wrld),                // objectToWorld
        new Transform(),                // worldToObject
        glm::vec3(1.f, 0.f, 0.f),        // albedo
        false,                          // reverseOrientation
        5.f,                            // radius
        -5.f,
        5.f,
        360.f
    ));
    */

    glm::mat4 obj2Wrld = glm::translate(glm::mat4(1.f), glm::vec3(0.f, -8.5f, 0.f));
    obj2Wrld = glm::rotate(obj2Wrld, glm::radians(15.f), glm::vec3(0.f, 1.f, 0.f));
    obj2Wrld = glm::scale(obj2Wrld, glm::vec3(100.f));
    std::vector< std::shared_ptr<Shape> > shapes = CreateTriangleMesh("bunny.obj",
        new Transform(obj2Wrld), new Transform(), glm::vec3(1.f, 1.f, 0.f));

    if (!shapes.empty())
    {
        scene.m_Shapes.insert(scene.m_Shapes.end(), shapes.begin(), shapes.end());
    }

    scene.m_Shapes.push_back(std::make_unique<Sphere>
    (
        new Transform(glm::translate(glm::mat4(1.f), glm::vec3(0.f, -1000.f, 0.f))),
        new Transform(glm::translate(glm::mat4(1.f), glm::vec3(0.f, 1000.f , 0.f))),
        glm::vec3(0.4f, 0.9f, 0.4f),     // albedo
        false,
        995.f,                          // radius
        -1000.f,
        1000.f,
        360.f
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
        glm::vec3(0.f, 1.f, 0.f),       // up
        glm::vec2(640, 400)
    );
}

}   // namespace Hikari