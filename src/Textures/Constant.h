#pragma once

#include "Texture.h"

namespace Hikari
{
	template <typename T>
	class ConstantTexture : public Texture<T>
	{
	public:
		ConstantTexture(const T& value) : m_Value(value) {}

		T Evaluate(const glm::vec2& uv) const override
		{
			return m_Value;
		}

	private:
		T m_Value;
	};

}	// namespace Hikari