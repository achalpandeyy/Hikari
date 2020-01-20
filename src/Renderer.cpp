#include "Renderer.h"

#include "Ray.h"
#include "Whitted.h"
#include "Sphere.h"
#include "PointLight.h"
#include "Primitive.h"
#include "DirectionalLight.h"
#include "BlockGenerator.h"
#include "ImageBlock.h"
#include "Transform.h"
#include "Triangle.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <tbb/tbb.h>

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

    m_RTEngine = std::make_shared<EmbreeRTEngine>();
    m_Integrator = std::make_unique<WhittedIntegrator>(m_RTEngine);
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

            // glm::vec3 color = m_Integrator->Li(primaryRay);
            glm::vec3 color = m_Integrator->Li(primaryRay, *m_Scene);
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
    // Red Sphere
    std::shared_ptr<Shape> redSphereShape = std::make_shared<Sphere>
    (
        new Transform(),
        new Transform(),
        false,
        5.f,
        -5.f,
        5.f,
        360.f
    );
    glm::vec3 redSphereAlbedo(1.f, 0.f, 0.f);
    std::shared_ptr<Primitive> redSphere = std::make_shared<GeometricPrimitive>(
        redSphereShape, redSphereAlbedo);
    scene.m_Primitives.push_back(redSphere);

    // Green Ground Sphere
    std::shared_ptr<Shape> greenSphereShape = std::make_shared<Sphere>
    (
        new Transform(glm::translate(glm::mat4(1.f), glm::vec3(0.f, -1000.f, 0.f))),
        new Transform(glm::translate(glm::mat4(1.f), glm::vec3(0.f, 1000.f, 0.f))),
        false,
        995.f,
        -1000.f,
        1000.f,
        360.f
    );
    glm::vec3 greenSphereAlbedo(0.4f, 0.9f, 0.4f);
    std::shared_ptr<Primitive> greenSphere = std::make_shared<GeometricPrimitive>(
        greenSphereShape, greenSphereAlbedo);
    scene.m_Primitives.push_back(greenSphere);

    /*
    // Triangle Mesh
    glm::mat4 obj2Wrld = glm::translate(glm::mat4(1.f), glm::vec3(0.f, -8.5f, 0.f));
    obj2Wrld = glm::rotate(obj2Wrld, glm::radians(15.f), glm::vec3(0.f, 1.f, 0.f));
    obj2Wrld = glm::scale(obj2Wrld, glm::vec3(100.f));
    std::vector< std::shared_ptr<Shape> > shapes = CreateTriangleMesh("bunny.obj",
        new Transform(obj2Wrld), new Transform());

    std::vector< std::shared_ptr<GeometricPrimitive> > primitives(shapes.size());
    for (size_t i = 0; i < shapes.size(); ++i)
    {
        std::shared_ptr<GeometricPrimitive> triangle = std::make_shared<GeometricPrimitive>
        (
            shapes[i],
            glm::vec3(1.f, 1.f, 0.f)
        );
        primitives[i] = triangle;
    }

    if (!primitives.empty())
    {
        scene.m_Primitives.insert(scene.m_Primitives.end(), primitives.begin(),
            primitives.end());
    }
    */

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