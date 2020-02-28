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
        glm::vec2 GetSample();

        const unsigned int m_NumSamples;

    private:
        float Real();

        std::mt19937 m_Engine;
        std::random_device m_RandomDevice;
    };

}   // namespace Hikari
