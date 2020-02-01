#pragma once

#include "RNG.h"

#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace Hikari
{
    // Care should be taken while using this class that all dimensions of the sample
    // are consumed appropriately. More info at PBRT 7.2.2
    class Sampler
    {
    public:
        Sampler::Sampler(uint64_t samplesPerPixel) : m_SamplesPerPixel(samplesPerPixel) {}

        virtual ~Sampler() {}

        // Start sampling for this pixel at position `pos`.
        virtual void StartPixel(const glm::vec2& pos);

        // Get the next dimension of the current sample.
        virtual float Get1D() = 0;

        // Get the next two dimensions of the current sample.
        virtual glm::vec2 Get2D() = 0;

        // PBRT returns a `CameraSample` struct here which consists of more than just a point,
        // but I'm returning a only a point here because that's only what is needed in Hikari for now.
        // PBRT 7.2.2
        glm::vec2 GetCameraSample(const glm::vec2& rasterPos) { return rasterPos + Get2D(); }

        // Record requests of arrays of sample components.
        void Request1DArray(unsigned int n);
        void Request2DArray(unsigned int n);

        // A way to "round" the number of requested samples to number which the sampler
        // can generate more efficiently.
        // The count returned by this method should be used for requesting samples.
        virtual unsigned int RoundCount(unsigned int n) const { return n; }

        // Access the requests recorded by Request(1/2)DArray methods.
        const float* Get1DArray(unsigned int n);
        const glm::vec2* Get2DArray(unsigned int n);

        virtual bool StartNextSample();
        
        // Samplers are not thread safe, so we clone the initial state of the sampler
        // so that it can used by multiple threads.
        virtual std::unique_ptr<Sampler> Clone(int seed) = 0;

        // Set the index of the sample in the current pixel to generate next.
        virtual bool SetSampleNumber(uint64_t sampleNum);

        // Number of samples that would be generated for each pixel in the final image.
        const uint64_t m_SamplesPerPixel;

    protected:
        glm::vec2 m_CurrentPixel;
        uint64_t m_CurrentPixelSampleIndex;
        std::vector<size_t> m_Samples1DArraySizes, m_Samples2DArraySizes;
        std::vector< std::vector<float> > m_SampleArray1D;
        std::vector< std::vector<glm::vec2> > m_SampleArray2D;

    private:
        size_t m_Array1DOffset, m_Array2DOffset;
    };

    // Base class for algorithms which can generate all dimensions of the sample at
    // the same time, rather than incrementally.
    class PixelSampler : public Sampler
    {
    public:
        // The param `numSampledDimensions` is the max number of dimensions for which
        // non-array sample values will be computed by the Sampler.
        PixelSampler(uint64_t samplesPerPixel, unsigned int numSampledDimensions);

        bool StartNextSample() override;
        bool SetSampleNumber(uint64_t sampleNum) override;

        float Get1D() override;
        glm::vec2 Get2D() override;

    protected:
        std::vector< std::vector<float> > m_Samples1D;
        std::vector< std::vector<glm::vec2> > m_Samples2D;
        unsigned int m_Current1DDimension = 0u, m_Current2DDimension = 0u;
        RNG m_RNG;
    };

}   // namespace Hikari
