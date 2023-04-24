#pragma once
#include "Core.h"

#include "CoreComponents\UIComponents.h"

namespace ecs
{
	struct UIStateComponent final : public nabi::ECS::ComponentBase
	{
		std::stack<UISceneComponent> m_UIScenes;
	};
} // namespace ecs
