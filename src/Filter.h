#pragma once

#include <glm/glm.hpp>

namespace Hikari
{

    class Filter
    {
    public:
        Filter(const glm::vec2& extent) : m_Extent(extent), m_InvExtent(1.f / extent) {}
        virtual ~Filter() {}

        // The param `pos` gives position of the point at which filter function is being asked to
        // evaluate relative to the center of the filter.
        virtual float Evaluate(const glm::vec2& pos) const = 0;

        const glm::vec2 m_Extent, m_InvExtent;
    };

}   // namespace Hikari