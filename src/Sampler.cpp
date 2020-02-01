#include "Sampler.h"

namespace Hikari
{

    void Sampler::StartPixel(const glm::vec2& pos)
    {
        m_CurrentPixel = pos;
        m_CurrentPixelSampleIndex = 0;

        // Reset array offsets for next pixel sample
        m_Array1DOffset = 0;
        m_Array2DOffset = 0;
    }

    void Sampler::Request1DArray(unsigned int n)
    {
        m_Samples1DArraySizes.push_back(n);
        m_SampleArray1D.push_back(std::vector<float>(n * m_SamplesPerPixel));
    }

    void Sampler::Request2DArray(unsigned int n)
    {
        m_Samples2DArraySizes.push_back(n);
        m_SampleArray2D.push_back(std::vector<glm::vec2>(n * m_SamplesPerPixel));
    }

    const float* Sampler::Get1DArray(unsigned int n)
    {
        if (m_Array1DOffset == m_SampleArray1D.size())
            return nullptr;

        return &m_SampleArray1D[m_Array1DOffset++][m_CurrentPixelSampleIndex * n];
        // return m_SampleArray1D[m_Array1DOffset++].data();
    }

    const glm::vec2* Sampler::Get2DArray(unsigned int n)
    {
        if (m_Array2DOffset == m_SampleArray2D.size())
            return nullptr;

        return &m_SampleArray2D[m_Array2DOffset++][m_CurrentPixelSampleIndex * n];
    }

    bool Sampler::StartNextSample()
    {
        // Reset array offsets for next pixel sample
        m_Array1DOffset = 0;
        m_Array2DOffset = 0;

        return ++m_CurrentPixelSampleIndex < m_SamplesPerPixel;
    }

    bool Sampler::SetSampleNumber(uint64_t sampleNum)
    {
        // Reset array offsets for next pixel sample
        m_Array1DOffset = 0;
        m_Array2DOffset = 0;

        m_CurrentPixelSampleIndex = sampleNum;
        return m_CurrentPixelSampleIndex < m_SamplesPerPixel;
    }

    PixelSampler::PixelSampler(uint64_t samplesPerPixel, unsigned int numSampledDimensions)
        : Sampler(samplesPerPixel)
    {
        for (unsigned int i = 0u; i < numSampledDimensions; ++i)
        {
            m_Samples1D.push_back(std::vector<float>(samplesPerPixel));
            m_Samples2D.push_back(std::vector<glm::vec2>(samplesPerPixel));
        }
    }

    bool PixelSampler::StartNextSample()
    {
        m_Current1DDimension = 0u;
        m_Current2DDimension = 0u;
        return Sampler::StartNextSample();
    }

    bool PixelSampler::SetSampleNumber(uint64_t sampleNum)
    {
        m_Current1DDimension = 0u;
        m_Current2DDimension = 0u;
        return Sampler::SetSampleNumber(sampleNum);
    }

    float PixelSampler::Get1D()
    {
        if (m_Current1DDimension < m_Samples1D.size())
            return m_Samples1D[m_Current1DDimension++][m_CurrentPixelSampleIndex];
        else
            return m_RNG.UniformFloat();
    }

    glm::vec2 PixelSampler::Get2D()
    {
        if (m_Current2DDimension < m_Samples2D.size())
            return m_Samples2D[m_Current2DDimension++][m_CurrentPixelSampleIndex];
        else
            return glm::vec2(m_RNG.UniformFloat(), m_RNG.UniformFloat());
    }

}   // namespace Hikari