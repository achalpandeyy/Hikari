#pragma once

#include "Filter.h"

#include <glm/glm.hpp>

#include <cmath>

namespace Hikari
{

    class MitchellFilter : public Filter
    {
    public:
        MitchellFilter(const glm::vec2& extent, float b, float c)
            : Filter(extent), m_B(b), m_C(c)
        {}

        float Evaluate(const glm::vec2& pos) const override;

    private:
        float Mitchell1D(float x) const;

        const float m_B, m_C;
    };

}   // namespace Hikari
