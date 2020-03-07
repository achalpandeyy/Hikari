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
			: SamplerIntegrator(camera), m_LightStrategy(strategy), m_MaxDepth(maxDepth)
		{}

		glm::vec3 Li(const Ray& ray, const Scene& scene, Sampler& sampler, unsigned int depth = 0) const override;

	private:
		LightStrategy m_LightStrategy;
		unsigned int m_MaxDepth;
	};

}	// namespace Hikari