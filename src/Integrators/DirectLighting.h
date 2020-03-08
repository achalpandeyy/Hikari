#pragma once

#include "Core/Integrator.h"

#include <glm/glm.hpp>

#include <memory>

namespace Hikari
{
	class Camera;

	enum class LightStrategy
	{
		UniformSampleAll,
		UniformSampleOne
	};

	class DirectLightingIntegrator : public SamplerIntegrator
	{
	public:
		DirectLightingIntegrator(LightStrategy strategy, unsigned int maxDepth, std::shared_ptr<const Camera> camera)
			: SamplerIntegrator(maxDepth, camera), m_LightStrategy(strategy)
		{}

		glm::vec3 Li(const Ray& ray, const Scene& scene, Sampler& sampler) const override;

	private:
		LightStrategy m_LightStrategy;
	};

}	// namespace Hikari