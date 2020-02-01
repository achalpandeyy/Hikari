#define _USE_MATH_DEFINES
#include "Sinc.h"

#include <cmath>

namespace Hikari
{

    float LanczosSincFilter::Evaluate(const glm::vec2& pos) const
    {
        return WindowedSinc(pos.x, m_Extent.x) * WindowedSinc(pos.y, m_Extent.y);
    }

    float LanczosSincFilter::Sinc(float x) const
    {
        x = std::abs(x);

        if (x < 1e-5f) return 1.f;

        return std::sin(M_PI * x) / (M_PI * x);
    }

    float LanczosSincFilter::WindowedSinc(float x, float extent) const
    {
        x = std::abs(x);

        if (x > extent) return 0.f;

        float lanczos = Sinc(x / m_Tau);
        return Sinc(x) * lanczos;
    }

}   // namespace Hikari