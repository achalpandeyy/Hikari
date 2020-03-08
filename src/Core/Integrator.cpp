// TODO(achal): Deal with defining NOMINMAX properly
#define NOMINMAX

#include "Integrator.h"

#include "BlockGenerator.h"
#include "Core/Camera.h"
#include "Core/Interaction.h"
#include "Core/Sampler.h"
#include "Core/Scene.h"
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
                    const glm::vec2 sample = sampler.GetSample2D();
                    Ray primaryRay = m_Camera->SpawnRay(rasterCoordinates + glm::vec2(row, col) + sample);

                    color += Li(primaryRay, scene, sampler);
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

    glm::vec3 UniformSampleAllLights(const Interaction& illumPoint, const Scene& scene, Sampler& sampler)
    {
        glm::vec3 L(0.f);

        for (size_t i = 0; i < scene.m_Lights.size(); ++i)
        {
            const std::shared_ptr<Light>& light = scene.m_Lights[i];

            glm::vec3 Ld(0.f);

            for (unsigned int i = 0; i < light->m_NumSamples; ++i)
            {
                Ld += EstimateDirect(illumPoint, *light, scene, sampler);
            }

            L += Ld / static_cast<float>(light->m_NumSamples);
        }

        return L;
    }

    glm::vec3 UniformSampleOneLight(const Interaction& illumPoint, const Scene& scene, Sampler& sampler)
    {
        size_t numLights = scene.m_Lights.size();

        if (numLights == 0)
            return glm::vec3(0.f);

        size_t lightIdx = std::min((size_t)sampler.GetSample1D() * numLights, numLights - 1);
        const std::shared_ptr<Light>& light = scene.m_Lights[lightIdx];

        return static_cast<float>(numLights) * EstimateDirect(illumPoint, *light, scene, sampler);
    }

    glm::vec3 EstimateDirect(const Interaction& illumPoint, const Light& light, const Scene& scene, Sampler& sampler)
    {
        glm::vec3 Ld(0.f);

        // Sample the light source
        //
        glm::vec3 wi;
        float lightPdf = 0.f;
        VisibilityTester visibility;

        const glm::vec2& lightSample = sampler.GetSample2D();
        glm::vec3 Li = light.Sample_Li(illumPoint, lightSample, &wi, &lightPdf, &visibility);

        if (lightPdf > 0.f && Li != glm::vec3(0.f))
        {
            // TODO(achal): Make a BSDF member in Interaction quickly!
            glm::vec3 f = illumPoint.m_Primitive->m_Material->ComputeScatteringFunctions(illumPoint)
                ->Evaluate(illumPoint.m_wo, wi) * glm::abs(glm::dot(wi, illumPoint.m_Normal));

            if (f != glm::vec3(0.f))
            {
                if (!visibility.Unoccluded(scene))
                    Li = glm::vec3(0.f);

                if (Li != glm::vec3(0.f))
                    Ld += f * Li / lightPdf;
            }
        }

        return Ld;
    }

}	// namespace Hikari