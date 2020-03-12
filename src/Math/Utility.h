#pragma once

#include <glm/glm.hpp>

namespace Hikari
{
	// TODO(achal): This is not the appropriate place for this function, move it somewhere else.
	void OrthonormalBasis(const glm::vec3& axisY, glm::vec3& axisX, glm::vec3& axisZ);

}	// namespace Hikari