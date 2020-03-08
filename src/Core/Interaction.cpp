#include "Interaction.h"

#include "Core/Primitive.h"
#include "Math/Constants.h"

namespace Hikari
{
	Ray Interaction::SpawnRayTo(const Interaction& i) const
	{
		return Ray(m_Position + EPSILON * m_Normal, glm::normalize(i.m_Position - m_Position),
			glm::distance(i.m_Position, m_Position) - 3.f * EPSILON);
	}

	Ray Interaction::SpawnRay(const glm::vec3& direction) const
	{
		return Ray(m_Position + EPSILON * m_Normal, direction);
	}

	glm::vec3 Interaction::Le() const
	{
		return m_Primitive->m_AreaLight ? m_Primitive->m_AreaLight->L(*this, m_wo): glm::vec3(0.f);
	}

}	// namespace Hikari