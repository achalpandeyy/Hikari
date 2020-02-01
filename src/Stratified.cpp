#include "Stratified.h"

#include "Sampling.h"

#include <algorithm>


namespace Hikari
{

    void StratifiedSampler::StartPixel(const glm::vec2& pos)
    {
        // Generate single stratified samples for the pixel
        for (size_t i = 0; i < m_Samples1D.size(); ++i)
        {
            StratifiedSample1D(&m_Samples1D[i][0], m_XPixelSamples * m_YPixelSamples, m_RNG, m_JitterSamples);
            Shuffle(&m_Samples1D[i][0], m_XPixelSamples * m_YPixelSamples, 1, m_RNG);
        }

        for (size_t i = 0; i < m_Samples2D.size(); ++i)
        {
            StratifiedSample2D(&m_Samples2D[i][0], m_XPixelSamples, m_YPixelSamples, m_RNG, m_JitterSamples);
            Shuffle(&m_Samples2D[i][0], m_XPixelSamples * m_YPixelSamples, 1, m_RNG);
        }

        // Generate arrays of stratified samples for the pixel
        for (size_t i = 0u; i < m_Samples1DArraySizes.size(); ++i)
        {
            for (uint64_t j = 0u; j < m_SamplesPerPixel; ++j)
            {
                size_t count = m_Samples1DArraySizes[i];
                StratifiedSample1D(&m_SampleArray1D[i][j * count], count, m_RNG, m_JitterSamples);
                Shuffle(&m_SampleArray1D[i][j * count], count, 1u, m_RNG);
            }
        }

        for (size_t i = 0u; i < m_Samples2DArraySizes.size(); ++i)
        {
            for (uint64_t j = 0u; j < m_SamplesPerPixel; ++j)
            {
                size_t count = m_Samples2DArraySizes[i];
                LatinHypercube(&m_SampleArray2D[i][j * count].x, count, 2u, m_RNG);
            }
        }

        PixelSampler::StartPixel(pos);
    }

    std::unique_ptr<Sampler> StratifiedSampler::Clone(int seed)
    {
        StratifiedSampler* ss = new StratifiedSampler(*this);

        // Questions 7.3 [2]
        ss->m_RNG.SetSequence(seed);
        return std::unique_ptr<Sampler>(ss);
    }

}   // namespace Hikari