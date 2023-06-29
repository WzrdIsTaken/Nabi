#pragma once
#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

namespace ecs::SComp
{
	struct DemoPropertiesComponent final : nabi::ECS::ComponentBase
	{
		unsigned int m_AsteroidsPerAxis;
		float m_AsteroidSpacing;

		DemoPropertiesComponent()
			: m_AsteroidsPerAxis(0u)
			, m_AsteroidSpacing(0.0f)
		{
		}
	};
} // namespace ecs::SComp

#endif // ifdef INCLUDE_DEMO
