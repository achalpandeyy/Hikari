#include "BSDF.h"

#include "Interaction.h"

namespace Hikari
{
	// TODO(achal): This is not the appropriate place for this function, move it somewhere else.
	void OrthonormalBasis(const glm::vec3& axisY, glm::vec3& axisX, glm::vec3& axisZ)
	{
		if (axisY.x < axisY.y)
			axisX = glm::normalize(glm::vec3(0.f, -axisY.z, axisY.y));
		else
			axisX = glm::normalize(glm::vec3(axisY.z, 0.f, -axisY.x));

		axisZ = glm::cross(axisX, axisY);
	}

	BSDF::BSDF(const Interaction& interaction, std::unique_ptr<BxDF> bxdf)
		: m_Normal(interaction.m_Normal), m_BxDF(std::move(bxdf))
	{
		OrthonormalBasis(m_Normal, m_Tangent, m_BiTangent);
	}

	glm::vec3 BSDF::Evaluate(const glm::vec3& woWorld, const glm::vec3& wiWorld) const
	{
		glm::vec3 wo = WorldToLocal(woWorld), wi = WorldToLocal(wiWorld);
		return m_BxDF->Evaluate(wo, wi);
	}

	void BSDF::Sample(glm::vec3& wiWorld, const glm::vec2& sample, float& pdf) const
	{
		glm::vec3 wi;
		// m_BxDF->Sample(WorldToLocal(woWorld), wi, sample, pdf);
		m_BxDF->Sample(wi, sample, pdf);
		wiWorld = LocalToWorld(wi);
	}

	glm::vec3 BSDF::LocalToWorld(const glm::vec3& v) const
	{
		return glm::vec3
		(
			m_Tangent.x * v.x + m_Normal.x * v.y + m_BiTangent.x * v.z,
			m_Tangent.y * v.x + m_Normal.y * v.y + m_BiTangent.y * v.z,
			m_Tangent.z * v.x + m_Normal.z * v.y + m_BiTangent.z * v.z
		);
	}

	glm::vec3 BSDF::WorldToLocal(const glm::vec3& v) const
	{
		return glm::vec3(glm::dot(v, m_Tangent), glm::dot(v, m_Normal), glm::dot(v, m_BiTangent));
	}

}	// namespace Hikari