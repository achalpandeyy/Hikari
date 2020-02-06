#include "Renderer.h"

#include "BlockGenerator.h"
#include "ImageBlock.h"
#include "PathIntegrator.h"
#include "Ray.h"
#include "Sampler.h"
#include "Transform.h"
#include "Whitted.h"

#include <glm/glm.hpp>
#include <tbb/tbb.h>

#include <chrono>
#include <iostream>
#include <random>
#include <thread>

namespace Hikari
{
    const glm::ivec2 resolution(640u, 400u);

Renderer::Renderer()
{
    // TODO(achal): Not make the path to scene a `nullptr` and actually
    // parse scene files to make scenes
    const char* path = nullptr;
    m_Scene = std::make_shared<Scene>(path);

    m_Integrator = std::make_unique<PathIntegrator>();
    m_Camera = std::make_unique<Camera>
    (
        glm::vec3(0.f, 0.f, 20.f),      // camera position
        glm::vec3(0.f, 0.f, 0.f),       // look at
        glm::vec3(0.f, 1.f, 0.f),       // up
        resolution
    );
}

void Renderer::Render() const
{   
    auto startTime = std::chrono::high_resolution_clock::now();

    BlockGenerator blockGenerator(resolution, BLOCK_DIMENSION);
    ImageBlock outputImage(resolution);

    std::thread renderThread([&]
    {
        std::cout << "Rendering.." << std::endl;

        tbb::blocked_range<unsigned int> range(0u, blockGenerator.GetBlockCount());

        auto map = [&](const tbb::blocked_range<unsigned int>& range)
        {
            ImageBlock imageBlock(glm::vec2(BLOCK_DIMENSION));

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

    const unsigned int numSamples = 512u;
    Sampler sampler(numSamples);

    for (unsigned int row = 0; row < block.m_Dimensions.y; ++row)
    {
        for (unsigned int col = 0; col < block.m_Dimensions.x; ++col)
        {
            int seed = row * BLOCK_DIMENSION + col;
            sampler.StartPixel(seed);

            glm::vec3 color(0.f);
            
            for (unsigned int i = 0u; i < sampler.m_NumSamples; ++i)
            {
                const glm::vec2 sample = sampler.GetSample();
                Ray primaryRay = m_Camera->SpawnRay(rasterCoordinates + glm::vec2(row, col) + sample);
                // Ray primaryRay = m_Camera->SpawnRay(glm::vec2(rasterCoordinates.x + row + u, rasterCoordinates.y + col + v));

                color += m_Integrator->Li(primaryRay, sampler, m_Scene);
            }

            // TODO(achal): This way of getting the index seems inconsistent and unintuitive.
            // The commented out line just below it should be the proper way but it doesn't work,
            // probably because there is some other inconsistency somewhere in the system. Fix.
            unsigned int idx = col * block.m_Dimensions.x + row;
            // unsigned int idx = row * block.m_Dimensions.x + col;

            block.m_Data[idx] = color / glm::vec3(sampler.m_NumSamples);
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
        m_Camera->SetResolution(glm::vec2(640, 400));
        break;
    case _320x200_:
        std::cout << "320 x 200" << std::endl;
        m_Camera->SetResolution(glm::vec2(320, 200));
        break;
    case _1x1_:
        std::cout << "1 x 1" << std::endl;
        m_Camera->SetResolution(glm::vec2(1, 1));
        break;
    }
}

glm::vec2 Renderer::GetResolution() const
{
    return m_Camera->GetResolution();
}

}   // namespace Hikari