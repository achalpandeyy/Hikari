#include "BSDF.h"

#include "Interaction.h"

#include <algorithm>

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

	BSDF::BSDF(const Interaction& interaction) : m_Normal(interaction.m_Normal)
	{
		OrthonormalBasis(m_Normal, m_Tangent, m_BiTangent);
	}

	BSDF::~BSDF()
	{
		for (unsigned int i = 0u; i < m_NumBxDFs; ++i)
			delete m_BxDFs[i];
	}

	glm::vec3 BSDF::f(const glm::vec3& woWorld, const glm::vec3& wiWorld, BxDFType flags) const
	{
		glm::vec3 wo = WorldToLocal(woWorld), wi = WorldToLocal(wiWorld);

		bool reflect = glm::dot(woWorld, m_Normal) * glm::dot(wiWorld, m_Normal) > 0.f;

		glm::vec3 res(0.f);
		for (unsigned int i = 0u; i < m_NumBxDFs; ++i)
		{
			if (m_BxDFs[i]->MatchesFlags(flags) &&
				((reflect && (m_BxDFs[i]->m_Type & BxDF_REFLECTION)) ||
				(!reflect && (m_BxDFs[i]->m_Type & BxDF_TRANSMISSION))))
			{
				res += m_BxDFs[i]->f(wo, wi);
			}
		}
		return res;
	}

	glm::vec3 BSDF::Sample_f(
		const glm::vec3&	woWorld,
		glm::vec3*			wiWorld,
		const glm::vec2&	sample,
		float*				pdf,
		BxDFType			type,
		BxDFType*			sampledType) const
	{
		// Choose which BxDF to sample
		//
		unsigned int numMatchingComponents = NumComponentsOfType(type);

		if (numMatchingComponents == 0u)
		{
			*pdf = 0.f;
			if (sampledType)
				*sampledType = BxDFType(0);
			return glm::vec3(0.f);
		}

		unsigned int component = std::min((unsigned int)std::floor(sample[0] * numMatchingComponents), numMatchingComponents - 1);

		// Get BxDF pointer for chosen component
		//
		BxDF* bxdf = nullptr;
		unsigned int count = component;
		for (unsigned int i = 0u; i < m_NumBxDFs; ++i)
		{
			if (m_BxDFs[i]->MatchesFlags(type) && count-- == 0u)
			{
				bxdf = m_BxDFs[i];
				break;
			}
		}

		// Remap BxDF the sample `sample[0]` to [0, 1) x [0, 1)
		//
		glm::vec2 sampleRemapped(sample[0] * numMatchingComponents - component, sample[1]);

		// Sample the chosen BxDF
		//
		glm::vec3 wo = WorldToLocal(woWorld), wi;
		*pdf = 0.f;
		if (sampledType)
			*sampledType = bxdf->m_Type;

		glm::vec3 f = bxdf->Sample_f(wo, &wi, sampleRemapped, pdf, sampledType);

		if (*pdf == 0.f)
			return glm::vec3(0.f);

		*wiWorld = LocalToWorld(wi);

		// Compute overall pdf with all matching BxDFs, but skip for perfectly specular BxDF
		//
		if (!(bxdf->m_Type & BxDF_SPECULAR) && numMatchingComponents > 1)
		{
			for (unsigned int i = 0u; i < m_NumBxDFs; ++i)
			{
				if (m_BxDFs[i] != bxdf && m_BxDFs[i]->MatchesFlags(type))
					*pdf += m_BxDFs[i]->Pdf(wo, wi);
			}
		}

		if (numMatchingComponents > 1)
			*pdf /= numMatchingComponents;

		// Compute value of BSDF for sampled direction
		//
		if (!(bxdf->m_Type & BxDF_SPECULAR) && numMatchingComponents > 1)
		{
			f = glm::vec3(0.f);

			bool reflect = glm::dot(m_Normal, woWorld) * glm::dot(m_Normal, *wiWorld) > 0.f;

			for (unsigned int i = 0u; i < m_NumBxDFs; ++i)
			{
				if (m_BxDFs[i]->MatchesFlags(type) &&
					((reflect && (m_BxDFs[i]->m_Type & BxDF_REFLECTION)) ||
					(!reflect && (m_BxDFs[i]->m_Type & BxDF_TRANSMISSION))))
				{
					f += m_BxDFs[i]->f(wo, wi);
				}
			}
		}

		return f;
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