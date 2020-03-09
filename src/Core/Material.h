#pragma once

namespace Hikari
{
	class Interaction;

	class Material
	{
	public:
		virtual ~Material() {}

		virtual void ComputeScatteringFunctions(Interaction* interaction) const = 0;
	};

}	// namespace Hikari