#include "Matte.h"

#include "BxDFs/Lambertian.h"
#include "BxDFs/OrenNayar.h"
#include "Core/Interaction.h"

namespace Hikari
{
	std::unique_ptr<BSDF> MatteMaterial::ComputeScatteringFunctions(const Interaction& interaction) const
	{
		// TODO(achal): Clamp both of them to valid values
		glm::vec3 kd = m_Reflectivity->Evaluate(interaction.m_UV);
		if (kd == glm::vec3(0.f))
			return nullptr;

		float sigma = m_Sigma->Evaluate(interaction.m_UV);
		if (sigma == 0.f)
			return std::make_unique<BSDF>(interaction, std::make_unique<Lambertian>(kd));
		else
			return std::make_unique<BSDF>(interaction, std::make_unique<OrenNayar>(kd, sigma));
	}

}	// namespace Hikari