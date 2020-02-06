#pragma once

#include <glm/glm.hpp>

#include <random>

namespace Hikari
{
    class Sampler
    {
    public:
        Sampler(unsigned int numSamples) : m_NumSamples(numSamples) {}
        void StartPixel(int seed);
        glm::vec2 GetSample();

        const unsigned int m_NumSamples;

    private:
        float Real();

        std::mt19937 m_Engine;
    };

}   // namespace Hikari
