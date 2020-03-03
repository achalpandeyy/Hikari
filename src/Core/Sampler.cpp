#include "Sampler.h"

#include "Math/Constants.h"

namespace Hikari
{
    void Sampler::StartPixel()
    {
        m_Engine.seed(m_RandomDevice());
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