#include "Matte.h"

#include "BxDFs/Lambertian.h"
#include "BxDFs/OrenNayar.h"
#include "Core/Interaction.h"

namespace Hikari
{
	void MatteMaterial::ComputeScatteringFunctions(Interaction* interaction) const
	{
		interaction->m_BSDF = std::make_shared<BSDF>(*interaction);

		// TODO(achal): Clamp both of them to valid values
		glm::vec3 kd = m_Reflectivity->Evaluate(interaction->m_UV);
		float sigma = m_Sigma->Evaluate(interaction->m_UV);

		if (kd != glm::vec3(0.f))
		{
			if (sigma == 0.f)
				interaction->m_BSDF->Add(new Lambertian(kd));
			else
				interaction->m_BSDF->Add(new OrenNayar(kd, sigma));
		}
	}

}	// namespace Hikari