#pragma once

#include "BxDF.h"

#include <glm/glm.hpp>

#include <memory>

namespace Hikari
{
	class Interaction;

	class BSDF
	{
	public:
		BSDF(const Interaction& interaction);

		~BSDF();

		void Add(BxDF* bxdf)
		{
			assert(m_NumBxDFs < m_MaxBxDFs);

			m_BxDFs[m_NumBxDFs++] = bxdf;
		}

		inline unsigned int NumComponentsOfType(BxDFType flags = BxDF_ALL) const
		{
			unsigned int number = 0u;
			for (unsigned int i = 0u; i < m_NumBxDFs; ++i)
			{
				if (m_BxDFs[i]->MatchesFlags(flags))
					++number;
			}
			return number;
		}

		glm::vec3 f(const glm::vec3& woWorld, const glm::vec3& wiWorld, BxDFType flags = BxDF_ALL) const;

		glm::vec3 Sample_f(
			const glm::vec3&	woWorld,
			glm::vec3*			wiWorld,
			const glm::vec2&	sample,
			float*				pdf,
			BxDFType			type = BxDF_ALL,
			BxDFType*			sampledType = nullptr) const;

	private:
		glm::vec3 LocalToWorld(const glm::vec3& v) const;
		glm::vec3 WorldToLocal(const glm::vec3& v) const;

		const glm::vec3 m_Normal;
		glm::vec3 m_Tangent, m_BiTangent;
		std::unique_ptr<BxDF> m_BxDF;

		unsigned int m_NumBxDFs = 0u;
		static constexpr unsigned int m_MaxBxDFs = 8u;
		BxDF* m_BxDFs[m_MaxBxDFs];
	};

}	// namespace Hikari