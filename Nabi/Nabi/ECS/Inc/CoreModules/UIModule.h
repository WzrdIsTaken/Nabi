#pragma once
#include "Core.h"

#include "CoreSingletonComponents\UIStateComponent.h"

namespace ecs::UIModule
{
	inline UIStateComponent const& GetUIStateComponent(nabi::Context const& context)
	{
		return context.m_Registry.get<UIStateComponent>(context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Input));
	}
	inline UIStateComponent& GetUIStateComponent(nabi::Context& context)
	{
		return const_cast<UIStateComponent&>(GetUIStateComponent(const_cast<nabi::Context const&>(context)));
	}

	inline UIStateComponent const* const TryGetUIStateComponent(nabi::Context const& context)
	{
		return context.m_Registry.try_get<UIStateComponent>(context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Input));
	}
	inline UIStateComponent* const TryGetUIStateComponent(nabi::Context& context)
	{
		return const_cast<UIStateComponent* const>(TryGetUIStateComponent(const_cast<nabi::Context const&>(context)));
	}

	enum class GetMode : int
	{
		First,
		FirstEnabled,

		ENUM_COUNT
	};
	inline UISceneComponent::UISceneEntities const* const GetCurrentUISceneEntities(nabi::Context const& context, GetMode const getMode)
	{
		std::stack<UISceneComponent> const& uiScenes = GetUIStateComponent(context).m_UIScenes;
		UISceneComponent::UISceneEntities const* currentUISceneEntities = nullptr;

		if (!uiScenes.empty())
		{
			switch (getMode)
			{
				case GetMode::First:
					currentUISceneEntities = &uiScenes.top().m_UISceneEntities;
					break;
				case GetMode::FirstEnabled:
				{
					std::deque<UISceneComponent> const& uiSceneDeque = uiScenes._Get_container(); // hmm
					for (auto it = uiSceneDeque.rbegin(); it != uiSceneDeque.rend(); ++it)
					{
						if (it->m_Enabled)
						{
							currentUISceneEntities = &it->m_UISceneEntities;
							break;
						}
					}
					break;
				}
				default:
					ASSERT_FAIL("Using an unexpected GetMode!");
					break;
			}
		}

		return currentUISceneEntities;
	}
	inline UISceneComponent::UISceneEntities* const GetCurrentUISceneEntities(nabi::Context& context, GetMode const getMode)
	{
		return const_cast<UISceneComponent::UISceneEntities* const>(GetCurrentUISceneEntities(const_cast<nabi::Context const&>(context), getMode));
	}
} // namespace ecs::UIModule

