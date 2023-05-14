#pragma once
#include "Core.h"

namespace ecs::SComp
{
	struct LightStateComponent final : public nabi::ECS::ComponentBase
	{
		unsigned int m_LightCount;
	};
} // namespace ecs::SComp
