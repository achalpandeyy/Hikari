#pragma once

#include <glm/glm.hpp>

namespace Hikari
{
	template <typename T>
	class Texture
	{
	public:
		virtual ~Texture() {}

		virtual T Evaluate(const glm::vec2& uv) const = 0;
	};

}	// namespace Hikari