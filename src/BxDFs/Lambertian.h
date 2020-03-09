#pragma once

#include "Core/BxDF.h"
#include "Math/Constants.h"

namespace Hikari
{
	class Lambertian : public BxDF
	{
	public:
		Lambertian(const glm::vec3& reflectivity) : BxDF(BxDFType(BxDF_REFLECTION | BxDF_DIFFUSE)),
			m_Reflectivity(reflectivity) {}

		glm::vec3 f(const glm::vec3& wo, const glm::vec3& wi) const override { return m_Reflectivity * INVPI; }

	private:
		const glm::vec3 m_Reflectivity;
	};

}	// namespace Hikari