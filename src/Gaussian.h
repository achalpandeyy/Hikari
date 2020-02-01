#pragma once

#include "Filter.h"

#include <algorithm>

namespace Hikari
{

    class GaussianFilter : public Filter
    {
    public:
        GaussianFilter(const glm::vec2& extent, float falloff)
            : Filter(extent), m_Falloff(falloff), m_ExpX(std::exp(-m_Falloff * m_Extent.x * m_Extent.x)),
            m_ExpY(std::exp(-m_Falloff * m_Extent.y * m_Extent.y))
        {}

        float Evaluate(const glm::vec2& pos) const override;

    private:
        float Gaussian(float d, float expv) const
        {
            return std::max(0.f, float(std::exp(-m_Falloff * d * d) - expv));
        }

        const float m_Falloff;
        const float m_ExpX, m_ExpY;
    };

}   // namespace Hikari
