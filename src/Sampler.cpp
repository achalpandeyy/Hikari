#include "Sampler.h"

namespace Hikari
{
    void Sampler::StartPixel(int seed)
    {
        m_Engine.seed(seed);
    }

    glm::vec2 Sampler::GetSample()
    {
        return glm::vec2(Real(), Real());
    }

    float Sampler::Real()
    {
        const std::uniform_real_distribution<float> distribution(0, 1);
        return distribution(m_Engine);
    }

}   // namespace Hikari