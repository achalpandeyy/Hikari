#include "BxDF.h"

#include "Math/Constants.h"

#include <algorithm>

namespace Hikari
{
    // TODO(achal): This function does not belong here, move it somewhere else.
    glm::vec3 UniformSampleHemisphere(const glm::vec2& e)
    {
        float sinTheta = std::sqrt(std::max(0.f, 1.f - e[0] * e[0]));
        float phi = 2.f * PI * e[1];
        return glm::vec3(sinTheta * std::cos(phi), e[0], sinTheta * std::sin(phi));
    }

	void BxDF::Sample(glm::vec3& wi, const glm::vec2& e, float& pdf) const
	{
        wi = UniformSampleHemisphere(e);
        pdf = 0.5f * INVPI;
	}

}	// namespace Hikari