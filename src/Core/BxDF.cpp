#include "BxDF.h"

#include "Math/Constants.h"

namespace Hikari
{
	glm::vec3 BxDF::Sample_f(
		const glm::vec3&	wo,
		glm::vec3*			wi,
		const glm::vec2&	sample,
		float*				pdf,
		BxDFType*			sampledType) const
	{
		*wi = UniformSampleHemisphere(sample);
		// *wi = CosineSampleHemisphere(sample);

		if (wi->y < 0.f)
			wi->y *= -1.f;

		*pdf = Pdf(wo, *wi);

		return f(wo, *wi);
	}

	float BxDF::Pdf(const glm::vec3& wo, const glm::vec3& wi) const
	{
		return SameHemisphere(wo, wi) ? 0.5f * INVPI : 0.f;
		// return SameHemisphere(wo, wi) ? AbsCosTheta(wi) * INVPI : 0.f;
	}

}	// namespace Hikari