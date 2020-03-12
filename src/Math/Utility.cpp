#include "Utility.h"

namespace Hikari
{
	void OrthonormalBasis(const glm::vec3& axisY, glm::vec3& axisX, glm::vec3& axisZ)
	{
		if (axisY.x < axisY.y)
			axisX = glm::normalize(glm::vec3(0.f, -axisY.z, axisY.y));
		else
			axisX = glm::normalize(glm::vec3(axisY.z, 0.f, -axisY.x));

		axisZ = glm::cross(axisX, axisY);
	}

}	// namespace Hikari