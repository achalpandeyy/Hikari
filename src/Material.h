#pragma once

#include "Texture.h"

#include <memory>

namespace Hikari
{
	class Material
	{
	public:
		Material(std::shared_ptr< Texture<glm::vec3> > color) : m_Color(color) {}

		// Base color
		//
		const std::shared_ptr< Texture<glm::vec3> > m_Color = nullptr;
	};

}	// namespace Hikari