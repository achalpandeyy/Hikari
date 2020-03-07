#include "Sampler.h"

#include "Math/Constants.h"

namespace Hikari
{
    void Sampler::StartPixel()
    {
        m_Engine.seed(m_RandomDevice());
    }

    float Sampler::GetSample1D()
    {
        return m_Distribution(m_Engine);
    }

    glm::vec2 Sampler::GetSample2D()
    {
        return glm::vec2(GetSample1D(), GetSample1D());
    }

}   // namespace Hikari