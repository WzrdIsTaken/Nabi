#pragma once
#include "Core.h"

namespace ecs::SComp
{
	struct LightStateComponent final : public nabi::ECS::ComponentBase
	{
		unsigned int m_LightCount;
#ifndef USE_EVENT_SYSTEM_UPDATE
		bool m_UpdateLights;
#endif // ifndef USE_EVENT_SYSTEM_UPDATE
	};
} // namespace ecs::SComp
