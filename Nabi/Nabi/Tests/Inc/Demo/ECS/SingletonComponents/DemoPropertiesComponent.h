#pragma once
#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

namespace ecs::SComp
{
	struct DemoPropertiesComponent final : nabi::ECS::ComponentBase
	{
		unsigned int m_AsteriodCount;
		float m_AsteriodSpawnRadius;

		DemoPropertiesComponent()
			: m_AsteriodCount(0u)
			, m_AsteriodSpawnRadius(0.0f)
		{
		}
	};
} // namespace ecs::SComp

#endif // ifdef INCLUDE_DEMO
