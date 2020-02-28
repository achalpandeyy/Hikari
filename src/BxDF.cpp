#include "BxDF.h"

#include "Core/Constants.h"

#include <algorithm>

namespace Hikari
{
    // TODO(achal): This function does not belong here, move it somewhere else.
    glm::vec3 UniformSampleHemisphere(const glm::vec2& sample)
    {
        float sinTheta = std::sqrt(std::max(0.f, 1.f - sample[0] * sample[0]));
        return glm::vec3(sinTheta * std::cos(2.f * PI * sample[1]), sample[0],
            sinTheta * std::sin(2.f * PI * sample[1]));
    }

	void BxDF::Sample(glm::vec3& wi, const glm::vec2& sample, float& pdf) const
	{
        wi = UniformSampleHemisphere(sample);
        pdf = 0.5f * INVPI;
	}

}	// namespace Hikari