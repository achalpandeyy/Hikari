#pragma once

#include "BxDF.h"

#include <glm/glm.hpp>

#include <memory>

namespace Hikari
{
	class Interaction;

	class BSDF : public BxDF
	{
	public:
		BSDF(const Interaction& interaction, std::unique_ptr<BxDF> bxdf);

		glm::vec3 Evaluate(const glm::vec3& woWorld, const glm::vec3& wiWorld) const override;

		void Sample(glm::vec3& wiWorld, const glm::vec2& sample, float& pdf) const override;

	private:
		glm::vec3 LocalToWorld(const glm::vec3& v) const;
		glm::vec3 WorldToLocal(const glm::vec3& v) const;

		const glm::vec3 m_Normal;
		glm::vec3 m_Tangent, m_BiTangent;
		std::unique_ptr<BxDF> m_BxDF;				
	};

}	// namespace Hikari