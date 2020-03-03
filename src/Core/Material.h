#pragma once

#include "BSDF.h"

#include <memory>

namespace Hikari
{
	class Interaction;

	class Material
	{
	public:
		virtual ~Material() {}

		virtual std::unique_ptr<BSDF> ComputeScatteringFunctions(const Interaction& interaction) const = 0;
	};

}	// namespace Hikari