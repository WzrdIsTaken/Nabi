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
	UISceneComponent::UISceneEntities const* const GetCurrentUISceneEntities(nabi::Context const& context, GetMode const getMode);
	inline UISceneComponent::UISceneEntities* const GetCurrentUISceneEntities(nabi::Context& context, GetMode const getMode)
	{
		return const_cast<UISceneComponent::UISceneEntities* const>(GetCurrentUISceneEntities(const_cast<nabi::Context const&>(context), getMode));
	}

	bool CheckIfMouseIsOverElement(nabi::Context const& context, dx::XMFLOAT2 const elementPosition, dx::XMFLOAT2 const elementDimensions);
	bool CheckIfMouseIsOverElement(nabi::Context const& context, dx::XMFLOAT2 const mousePosition, dx::XMFLOAT2 const elementPosition, dx::XMFLOAT2 const elementDimensions);
} // namespace ecs::UIModule

