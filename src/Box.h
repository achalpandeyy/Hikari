#pragma once

#include "Filter.h"

namespace Hikari
{

    class BoxFilter : public Filter
    {
    public:
        BoxFilter(const glm::vec2& extent) : Filter(extent) {}

        float Evaluate(const glm::vec2& pos) const override;
    };

}   // namespace Hikari