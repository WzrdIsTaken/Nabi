#pragma once
#include "Core.h"

#include "CoreComponents\UIComponents.h"

namespace ecs
{
	struct UIStateComponent final : public nabi::ECS::ComponentBase
	{
		std::stack<UISceneComponent> m_UIScenes;
	};

	struct UIStorageComponent final : public nabi::ECS::ComponentBase
	{
		typedef unsigned int Sticky;
		std::unordered_map<Sticky, std::any> m_Storage;
	};
} // namespace ecs
