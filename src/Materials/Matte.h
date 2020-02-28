#pragma once

#include "BSDF.h"
#include "Material.h"
#include "Texture.h"

#include <memory>

namespace Hikari
{
	class Interaction;

	// This material is parameterized by a spectral diffuse reflection value `m_Reflectivity`
	// and a scalar rougness value `m_Sigma`.
	//
	class MatteMaterial : public Material
	{
	public:
		MatteMaterial(
			const std::shared_ptr< Texture<glm::vec3> >&	reflectivity,
			const std::shared_ptr< Texture<float> >&		sigma)
			: m_Reflectivity(reflectivity), m_Sigma(sigma)
		{}

		std::unique_ptr<BSDF> ComputeScatteringFunctions(const Interaction& interaction) const override;

	private:
		std::shared_ptr< Texture<glm::vec3> > m_Reflectivity = nullptr;
		std::shared_ptr< Texture<float> > m_Sigma = nullptr;
	};

}	// namespace Hikari