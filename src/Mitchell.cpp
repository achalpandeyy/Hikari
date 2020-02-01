#include "Mitchell.h"

namespace Hikari
{

    float MitchellFilter::Evaluate(const glm::vec2& pos) const
    {
        return Mitchell1D(pos.x * m_InvExtent.x) * Mitchell1D(pos.y * m_InvExtent.y);
    }

    float MitchellFilter::Mitchell1D(float x) const
    {
        x = std::abs(2 * x);
        if (x > 1)
        {
            return ((-m_B - 6 * m_C) * x * x * x + (6 * m_B + 30 * m_C) * x * x +
                (-12 * m_B - 48 * m_C) * x + (8 * m_B + 24 * m_C)) *
                (1.f / 6.f);
        }
        else
        {
            return ((12 - 9 * m_B - 6 * m_C) * x * x * x +
                (-18 + 12 * m_B + 6 * m_C) * x * x + (6 - 2 * m_B)) *
                (1.f / 6.f);
        }
    }

}   // namespace Hikari