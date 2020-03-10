#pragma once

#include "Core/Light.h"
#include "Core/Material.h"
#include "Core/Shape.h"

#include <memory>

namespace Hikari
{
	class Primitive
	{
	public:
		Primitive(
			const std::shared_ptr<Shape>&		shape,
			const std::shared_ptr<Material>&	material,
			const std::shared_ptr<AreaLight>&	areaLight)
			: m_Shape(shape), m_Material(material), m_AreaLight(areaLight)
		{}
		
		std::shared_ptr<Shape> m_Shape;
		std::shared_ptr<Material> m_Material;
		std::shared_ptr<AreaLight> m_AreaLight;
	};

}	// namespace Hikari