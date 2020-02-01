#include "Triangle.h"

#include <algorithm>

namespace Hikari
{

    float TriangleFilter::Evaluate(const glm::vec2& pos) const
    {
        // Questions 7.8.1 [1]
        return std::max(0.f, m_Extent.x - std::abs(pos.x)) * std::max(0.f, m_Extent.y - std::abs(pos.y));
    }

}   // namespace Hikari