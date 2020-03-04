#include "Integrator.h"

#include "BlockGenerator.h"
#include "Core/Camera.h"
#include "Core/Sampler.h"
#include "ImageBlock.h"
#include "Math/Ray.h"

#include <tbb/tbb.h>

#include <chrono>
#include <iostream>
#include <thread>

// TODO(achal): Put it elsewhere.
#define BLOCK_DIMENSION 32u

namespace Hikari
{
	void SamplerIntegrator::Render(const Scene& scene) 
	{
		auto startTime = std::chrono::high_resolution_clock::now();

        // TODO(achal): Put it elsewhere.
        const glm::vec2 resolution(1280, 720);

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
                    RenderBlock(imageBlock, scene);
                    outputImage.Put(imageBlock);
                }
            };

            tbb::parallel_for(range, map);
        });

        renderThread.join();
        auto stopTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);

        std::cout << "Rendered!" << std::endl;
        std::cout << "Render time: " << duration.count() / 1e6 << " seconds." << std::endl;

        outputImage.WriteImage();
	}

    void SamplerIntegrator::RenderBlock(ImageBlock& block, const Scene& scene) const
    {
        glm::vec2 rasterCoordinates(block.m_Position.x * BLOCK_DIMENSION, block.m_Position.y * BLOCK_DIMENSION);

        // TODO(achal): Put it elsewhere.
        const unsigned int depth = 50u;
        const unsigned int numSamples = 16u;
        Sampler sampler(numSamples);

        for (unsigned int row = 0; row < block.m_Dimensions.y; ++row)
        {
            for (unsigned int col = 0; col < block.m_Dimensions.x; ++col)
            {
                sampler.StartPixel();

                glm::vec3 color(0.f);

                for (unsigned int i = 0u; i < sampler.m_NumSamples; ++i)
                {
                    const glm::vec2 sample = sampler.GetSample();
                    Ray primaryRay = m_Camera->SpawnRay(rasterCoordinates + glm::vec2(row, col) + sample);

                    color += Li(primaryRay, sampler, scene, depth);
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

}	// namespace Hikari