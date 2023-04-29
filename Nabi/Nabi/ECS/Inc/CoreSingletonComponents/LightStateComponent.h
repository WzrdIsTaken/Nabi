#pragma once
#include "Core.h"

namespace ecs::SComp
{
	struct LightStateComponent final : public nabi::ECS::ComponentBase
	{
		unsigned int m_LightCount;
		bool m_UpdateLights;
	};
} // namespace ecs::SComp
