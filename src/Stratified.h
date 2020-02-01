#pragma once

#include "Sampler.h"

#include <memory>

namespace Hikari
{
    class StratifiedSampler : public PixelSampler
    {
    public:
        StratifiedSampler(
            unsigned int    xPixelSamples,
            unsigned int    yPixelSamples,
            bool            jitterSamples,
            unsigned int    numSampledDimensions)
            : PixelSampler((uint64_t)xPixelSamples * (uint64_t)yPixelSamples, numSampledDimensions),
            m_XPixelSamples(xPixelSamples), m_YPixelSamples(yPixelSamples), m_JitterSamples(jitterSamples)
        {}

        void StartPixel(const glm::vec2& pos) override;
        std::unique_ptr<Sampler> Clone(int seed) override;

    private:
        const unsigned int m_XPixelSamples, m_YPixelSamples;
        const bool m_JitterSamples;
    };

}   // namespace Hikari
