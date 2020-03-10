#include "DirectLighting.h"

#include "Core/Interaction.h"
#include "Core/Scene.h"

namespace Hikari
{
	glm::vec3 DirectLightingIntegrator::Li(
		const Ray&		ray,
		const Scene&	scene,
		Sampler&		sampler) const
	{
		glm::vec3 L(0.f);

		Interaction interaction;

		if (!scene.Intersect(ray, &interaction))
			return L;

		// TODO(achal): Make `ComputeScatteringFunctions` method for Interaction, and give Interaction
		// a member of type BSDF.
		// interaction.ComputScatteringFunctions(ray);

		interaction.ComputeScatteringFunctions();

		// Compute emitted light from this point of interaction, in case we hit an area light source.
		//
		L += interaction.Le();

		// Compute direct lighting
		//
		if (scene.m_Lights.size() > 0)
		{
			if (m_LightStrategy == LightStrategy::UniformSampleAll)
				L += UniformSampleAllLights(interaction, scene, sampler);

			else
				L += UniformSampleOneLight(interaction, scene, sampler);
		}

		return L;
	}

}	// namespace Hikari