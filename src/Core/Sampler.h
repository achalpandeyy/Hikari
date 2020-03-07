#pragma once

#include <glm/glm.hpp>

#include <random>

namespace Hikari
{
    glm::vec3 UniformSampleHemisphere(const glm::vec2& sample);

    class Sampler
    {
    public:
        Sampler(unsigned int numSamples) : m_NumSamples(numSamples) {}

        void StartPixel();

        float GetSample1D();

        glm::vec2 GetSample2D();

        const unsigned int m_NumSamples;

    private:
        std::mt19937 m_Engine;
        const std::uniform_real_distribution<float> m_Distribution{ 0.f, 1.f }; 
        std::random_device m_RandomDevice;
    };

}   // namespace Hikari
