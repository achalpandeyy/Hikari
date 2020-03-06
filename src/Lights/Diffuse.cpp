#include "Diffuse.h"

#include "Core/Interaction.h"
#include "Core/Shape.h"

namespace Hikari
{
	glm::vec3 DiffuseAreaLight::Sample_Li(
		const Interaction&	illumPoint,
		const glm::vec2&	e,
		glm::vec3*			wi,
		float*				pdf,
		VisibilityTester*	vis) const
	{
		Interaction pShape = m_Shape->Sample(illumPoint, e, pdf);

		if (*pdf == 0.f || glm::distance(pShape.m_Position, illumPoint.m_Position) == 0.f)
			return glm::vec3(0.f);

		*wi = glm::normalize(pShape.m_Position - illumPoint.m_Position);
		*vis = VisibilityTester(illumPoint, pShape);
		float cosTheta = glm::dot(-*wi, pShape.m_Normal);
		if (cosTheta < 0.f)
			cosTheta = 0.f;
		return L(pShape, -*wi) * cosTheta / glm::dot(pShape.m_Position - illumPoint.m_Position, pShape.m_Position - illumPoint.m_Position);
	}

	float DiffuseAreaLight::Pdf_Li(const Interaction& illumPoint, const glm::vec3& wi) const
	{
		return m_Shape->Pdf(illumPoint, wi);
	}

	glm::vec3 DiffuseAreaLight::L(const Interaction& pShape, const glm::vec3& w) const
	{
		return (m_TwoSided || glm::dot(pShape.m_Normal, w) > 0.f) ? m_LEmit : glm::vec3(0.f);
	}

}	// namespace Hikari