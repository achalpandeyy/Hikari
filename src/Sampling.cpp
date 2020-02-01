#include "Sampling.h"

namespace Hikari
{

    void StratifiedSample1D(float* samples, unsigned int numSamples, RNG& rng, bool jitter)
    {
        float invNumSamples = 1.f / float(numSamples);
        for (unsigned int i = 0u; i < numSamples; ++i)
        {
            float delta = jitter ? rng.UniformFloat() : 0.5f;
            samples[i] = std::min((i + delta) * invNumSamples, FLOAT_ONE_MINUS_EPSILON);
        }
    }

    void StratifiedSample2D(glm::vec2* samples, unsigned int numSamplesX, unsigned int numSamplesY, RNG& rng, bool jitter)
    {
        float dx = 1.f / float(numSamplesX), dy = 1.f / float(numSamplesY);
        for (unsigned int y = 0; y < numSamplesY; ++y)
        {
            for (unsigned int x = 0; x < numSamplesX; ++x)
            {
                float jx = jitter ? rng.UniformFloat() : 0.5f;
                float jy = jitter ? rng.UniformFloat() : 0.5f;
                samples->x = std::min((x + jx) * dx, FLOAT_ONE_MINUS_EPSILON);
                samples->y = std::min((y + jy) * dy, FLOAT_ONE_MINUS_EPSILON);
                samples++;
            }
        }
    }

    // Questions 7.3 [1]
    void LatinHypercube(float* samples, unsigned int numSamples, unsigned int numDimensions, RNG& rng)
    {
        // Generate LHS samples along diagonal
        float invNumSamples = 1.f / float(numSamples);
        for (unsigned int i = 0u; i < numSamples; ++i)
        {
            for (unsigned int j = 0u; j < numDimensions; ++j)
            {
                float sj = (i + rng.UniformFloat()) * invNumSamples;
                samples[i * numDimensions + j] = std::min(sj, FLOAT_ONE_MINUS_EPSILON);
            }
        }

        // Permute LHS samples in each dimension
        // This permutation is different from the `Shuffle` function template defined in `Sampling.h`,
        // as `Shuffle` rearranged only the samples and not the dimensions inside them.
        // Here we permute dimensions, so one dimension of a sample can end up in another sample.
        for (unsigned int i = 0u; i < numDimensions; ++i)
        {
            for (unsigned int j = 0u; j < numSamples; ++j)
            {
                unsigned int other = j + rng.UniformUInt32(numSamples - j);
                std::swap(samples[j * numDimensions + i], samples[other * numDimensions + i]);
            }
        }
    }

}   // namespace Hikari