#include "OrenNayar.h"

#include "Math/Constants.h"

#include <glm/glm.hpp>

#include <algorithm>

namespace Hikari
{
	OrenNayar::OrenNayar(const glm::vec3& reflectivity, float sigma)
		: m_Reflectivity(reflectivity), m_Sigma(glm::radians(sigma))
	{
		float sigmaSq = sigma * sigma;
		m_A = 1.f - (sigmaSq / (2.f * (sigmaSq + 0.33f)));
		m_B = 0.45f * sigmaSq / (sigmaSq + 0.09f);
	}

	glm::vec3 OrenNayar::Evaluate(const glm::vec3& wo, const glm::vec3& wi) const
	{
		float sinThetaI = SinTheta(wi);
		float sinThetaO = SinTheta(wo);

		float maxCos = 0.f;
		if (sinThetaI > 1e-4f && sinThetaO > 1e-4f)
		{
			float sinPhiI = SinPhi(wi), cosPhiI = CosPhi(wi);
			float sinPhiO = SinPhi(wo), cosPhiO = CosPhi(wo);
			float diffCos = cosPhiI * cosPhiO + sinPhiI * sinPhiO;
			maxCos = std::max(0.f, diffCos);
		}

		float sinAlpha = 0.f, tanBeta = 0.f;
		if (AbsCosTheta(wi) > AbsCosTheta(wo))
		{
			// thetaO > thetaI
			sinAlpha = sinThetaO;
			tanBeta = sinThetaI / AbsCosTheta(wi);
		}
		else
		{
			sinAlpha = sinThetaI;
			tanBeta = sinThetaO / AbsCosTheta(wo);
		}
		return m_Reflectivity * INVPI * (m_A + m_B * maxCos * sinAlpha * tanBeta);
	}


}	// namespace Hikari