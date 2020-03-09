#pragma once

#include "Core/Core.h"
#include "Math/Constants.h"

#include <glm/glm.hpp>

#include <algorithm>

namespace Hikari
{
	inline float CosTheta(const glm::vec3& w) { return w.y; }

	inline float CosSqTheta(const glm::vec3& w) { return w.y * w.y; }

	inline float AbsCosTheta(const glm::vec3& w) { return std::abs(w.y); }

	inline float SinSqTheta(const glm::vec3& w) { return std::max(0.f, 1.f - CosSqTheta(w)); }

	inline float SinTheta(const glm::vec3& w) { return std::sqrt(SinSqTheta(w)); }

	inline float TanTheta(const glm::vec3& w) { return SinTheta(w) / CosTheta(w); }

	inline float CosPhi(const glm::vec3& w)
	{
		float sinTheta = SinTheta(w);
		return (sinTheta == 0.f) ? 1.f : glm::clamp(w.x / sinTheta, -1.f, 1.f);
	}

	inline float SinPhi(const glm::vec3& w)
	{
		float sinTheta = SinTheta(w);
		return (sinTheta == 0.f) ? 0.f : glm::clamp(w.z / sinTheta, -1.f, 1.f);
	}

	inline float CosSqPhi(const glm::vec3& w) { return CosPhi(w) * CosPhi(w); }

	inline float SinSqPhi(const glm::vec3& w) { return SinPhi(w) * SinPhi(w); }

	inline bool SameHemisphere(const glm::vec3& w1, const glm::vec3& w2) { return w1.y * w2.y > 0.f; }

	// TODO(achal): The following two functions do not belong here, move them somewhere else.
	inline glm::vec3 UniformSampleHemisphere(const glm::vec2& sample)
	{
		float sinTheta = std::sqrt(std::max(0.f, 1.f - sample[0] * sample[0]));
		float phi = 2.f * PI * sample[1];

		return glm::vec3(sinTheta * std::cos(phi), sample[0], sinTheta * std::sin(phi));
	}

	inline glm::vec3 CosineSampleHemisphere(const glm::vec2& sample)
	{
		float cos2Theta = 1.f - 2.f * sample[0];
		float cosTheta = std::sqrt(std::max(0.f, (1.f + cos2Theta) / 2.f));
		float sinTheta = std::sqrt(std::max(0.f, (1.f - cos2Theta) / 2.f));
		float phi = 2.f * PI * sample[1];

		return glm::vec3(sinTheta * std::cos(phi), cosTheta, sinTheta * std::sin(phi));
	}

	enum BxDFType
	{
		BxDF_REFLECTION		= BIT(0),
		BxDF_TRANSMISSION	= BIT(1),
		BxDF_DIFFUSE		= BIT(2),
		BxDF_GLOSSY			= BIT(3),
		BxDF_SPECULAR		= BIT(4),
		BxDF_ALL			= BxDF_REFLECTION | BxDF_TRANSMISSION | BxDF_DIFFUSE | BxDF_GLOSSY | BxDF_SPECULAR
	};

	class BxDF
	{
	public:
		BxDF(BxDFType type) : m_Type(type) {}

		virtual ~BxDF() {}

		virtual glm::vec3 f(const glm::vec3& wo, const glm::vec3& wi) const = 0;

		virtual glm::vec3 Sample_f(
			const glm::vec3&	wo,
			glm::vec3*			wi,
			const glm::vec2&	sample,
			float*				pdf,
			BxDFType*			sampledType = nullptr) const;

		virtual float Pdf(const glm::vec3& wo, const glm::vec3& wi) const;

		inline bool MatchesFlags(BxDFType flags) const { return (flags & m_Type) == m_Type; }

		const BxDFType m_Type;
	};

}	// namespace Hikari