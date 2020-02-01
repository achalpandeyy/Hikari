#include "Gaussian.h"

#include <cmath>

namespace Hikari
{

    float GaussianFilter::Evaluate(const glm::vec2& pos) const
    {
        return Gaussian(pos.x, m_ExpX) * Gaussian(pos.y, m_ExpY);
    }

} // namespace Hikari