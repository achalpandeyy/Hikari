#pragma once

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

	class BxDF
	{
	public:
		virtual ~BxDF() {}

		virtual glm::vec3 Evaluate(const glm::vec3& wo, const glm::vec3& wi) const = 0;

		virtual void Sample(glm::vec3& wi, const glm::vec2& sample, float& pdf) const;
	};

}	// namespace Hikari