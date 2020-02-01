#pragma once

#include "Filter.h"

namespace Hikari
{

    class LanczosSincFilter : public Filter
    {
    public:
        LanczosSincFilter(const glm::vec2& extent, float tau)
            : Filter(extent), m_Tau(tau)
        {}

        float Evaluate(const glm::vec2& pos) const override;

    private:
        float Sinc(float x) const;
        float WindowedSinc(float x, float extent) const;

        const float m_Tau;
    };

}   // namespace Hikari