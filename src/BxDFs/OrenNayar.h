#pragma once

#include "BxDF.h"

namespace Hikari
{
	// A microfacet based reflection model with parameters:
	// `m_Reflectivity` - amount of light reflected.
	// `sigma` - the standard deviation of the microfacet orientation angle (in degrees).
	// Reference: http://www.pbr-book.org/3ed-2018/Reflection_Models/Microfacet_Models.html#OrenndashNayarDiffuseReflection
	//
	class OrenNayar : public BxDF
	{
	public:
		OrenNayar(const glm::vec3& reflectivity, float sigma);

		glm::vec3 Evaluate(const glm::vec3& wo, const glm::vec3& wi) const override;

	private:
		const glm::vec3 m_Reflectivity;
		const float m_Sigma;
		float m_A, m_B;
	};

}	// namespace Hikari