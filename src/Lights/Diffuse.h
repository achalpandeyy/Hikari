#pragma once

#include "Core/Light.h"

#include <glm/glm.hpp>

#include <memory>

namespace Hikari
{
	class Interaction;
	class Shape;

	class DiffuseAreaLight : public AreaLight
	{
	public:
		DiffuseAreaLight(
			unsigned int					numSamples,
			const glm::vec3&				LEmit,
			const std::shared_ptr<Shape>&	shape,
			bool							twoSided)
			: AreaLight(numSamples), m_LEmit(LEmit), m_Shape(shape), m_TwoSided(twoSided)
		{}

		glm::vec3 Sample_Li(
			const Interaction&	illumPoint,
			const glm::vec2&	e,
			glm::vec3*			wi,
			float*				pdf,
			VisibilityTester*	vis) const override;

		float Pdf_Li(const Interaction& illumPoint, const glm::vec3& wi) const override;

		glm::vec3 L(const Interaction& interaction, const glm::vec3& w) const override;

	private:
		const glm::vec3 m_LEmit;
		const std::shared_ptr<Shape> m_Shape;
		const bool m_TwoSided;
	};

}	// namespace Hikari