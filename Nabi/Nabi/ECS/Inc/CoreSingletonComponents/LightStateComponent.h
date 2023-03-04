#pragma once
#include "Core.h"

namespace ecs
{
	struct LightStateComponent final : public nabi::ECS::ComponentBase
	{
		int m_LightCount;
		bool m_UpdateLights;
	};
} // namespace ecs
