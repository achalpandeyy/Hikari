#include "Interaction.h"

#include "Math/Constants.h"

namespace Hikari
{
	Ray Interaction::SpawnRayTo(const Interaction& i) const
	{
		return Ray(m_Position + EPSILON * m_Normal, glm::normalize(i.m_Position - m_Position), glm::distance(i.m_Position, m_Position));
	}

}	// namespace Hikari