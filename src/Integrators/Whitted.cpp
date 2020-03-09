#include "Whitted.h"

#include "Core/BSDF.h"
#include "Core/Sampler.h"
#include "Core/Scene.h"
#include "Math/Ray.h"

namespace Hikari
{
	glm::vec3 WhittedIntegrator::Li(const Ray& ray, const Scene& scene, Sampler& sampler) const
	{
		glm::vec3 L(0.f);

		Interaction interaction = scene.Intersect(ray);

		if (!interaction.m_Primitive)
			return L;

		const glm::vec3& normal = interaction.m_Normal;
		glm::vec3 wo = interaction.m_wo;

		interaction.ComputeScatteringFunctions();

		// Compute emitted light from this point of interaction, in case we hit an area light source.
		//
		L += interaction.Le();

		for (const auto& light : scene.m_Lights)
		{
			glm::vec3 wi;
			float pdf = 0.f;
			VisibilityTester visibility;

			glm::vec3 Li = light->Sample_Li(interaction, sampler.GetSample2D(), &wi, &pdf, &visibility);

			if (Li == glm::vec3(0.f) || pdf == 0.f) continue;

			glm::vec3 f = interaction.m_BSDF->f(wo, wi);

			if (f != glm::vec3(0.f) && visibility.Unoccluded(scene))
				L += f * Li * glm::abs(glm::dot(wi, interaction.m_Normal)) / pdf;
		}

		return L;
	}

}   // namespace Hikari