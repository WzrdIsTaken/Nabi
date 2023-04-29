#pragma once
#include "Core.h"

#include "CoreSingletonComponents\UIStateComponent.h"

namespace ecs::UIModule
{
	// --- UI State ---

	inline SComp::UIStateComponent const& GetUIStateComponent(nabi::Context const& context)
	{
		return context.m_Registry.get<SComp::UIStateComponent>(context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Input));
	}
	inline SComp::UIStateComponent& GetUIStateComponent(nabi::Context& context)
	{
		return const_cast<SComp::UIStateComponent&>(GetUIStateComponent(const_cast<nabi::Context const&>(context)));
	}

	inline SComp::UIStateComponent const* const TryGetUIStateComponent(nabi::Context const& context)
	{
		return context.m_Registry.try_get<SComp::UIStateComponent>(context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Input));
	}
	inline SComp::UIStateComponent* const TryGetUIStateComponent(nabi::Context& context)
	{
		return const_cast<SComp::UIStateComponent* const>(TryGetUIStateComponent(const_cast<nabi::Context const&>(context)));
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

	// --- UI Storage ---

	inline SComp::UIStorageComponent const& GetUIStorageComponent(nabi::Context const& context)
	{
		return context.m_Registry.get<SComp::UIStorageComponent>(context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Input));
	}
	inline SComp::UIStorageComponent& GetUIStorageComponent(nabi::Context& context)
	{
		return const_cast<SComp::UIStorageComponent&>(GetUIStorageComponent(const_cast<nabi::Context const&>(context)));
	}

	template<typename T>
	T GetStickyAs(nabi::Context const& context, SComp::UIStorageComponent::Sticky const sticky)
	{
		std::any const sticky = GetSticky(context, sticky);

		// Debug flow
		ASSERT_CODE
		(
			try
			{
				T const stickyAsT = std::any_cast<T>(sticky);
				return stickyAsT;
			}
			catch (std::bad_any_cast const& exception)
			{
				ASSERT_FAIL("Couldn't get sticky " << sticky << " as type T!");
				return {};
			}
		)

		// Release flow
		FINAL_CODE
		(
			T const stickyAsT = std::any_cast<T>(sticky);
			return stickyAsT;
		)
	};
	inline std::any GetSticky(nabi::Context const& context, SComp::UIStorageComponent::Sticky const sticky)
	{ 
		SComp::UIStorageComponent const& uiStorage = GetUIStorageComponent(context);
		return uiStorage.m_Storage.at(sticky);
	};

	inline bool HasSticky(SComp::UIStorageComponent const& uiStorage, SComp::UIStorageComponent::Sticky const sticky)
	{
		return uiStorage.m_Storage.find(sticky) != uiStorage.m_Storage.end();
	}
	inline bool HasSticky(nabi::Context const& context, SComp::UIStorageComponent::Sticky const sticky)
	{ 
		SComp::UIStorageComponent const& uiStorage = GetUIStorageComponent(context);
		return HasSticky(uiStorage, sticky);
	};

	inline void SetSticky(nabi::Context& context, SComp::UIStorageComponent::Sticky const sticky, std::any const value)
	{
		SComp::UIStorageComponent& uiStorage = GetUIStorageComponent(context);
		if (HasSticky(uiStorage, sticky))
		{
			LOG(LOG_PREP, LOG_WARN, LOG_CATEGORY_UI << "Overriding an existing sticky's  (" << sticky << ") value!" << ENDLINE);
			uiStorage.m_Storage.at(sticky) = value;
		}
		else
		{
			std::pair<SComp::UIStorageComponent::Sticky, std::any> const pair = { sticky, value };
			uiStorage.m_Storage.emplace(pair);
		}
	}

	inline void ClearSticky(nabi::Context& context, SComp::UIStorageComponent::Sticky const sticky)
	{
		CONDITIONAL_LOG(!HasSticky(context, sticky), LOG_PREP, LOG_INFO, LOG_CATEGORY_UI << "Clearing a sticky (" << sticky << ") which doesn't exist!" << ENDLINE);

		SComp::UIStorageComponent& uiStorage = GetUIStorageComponent(context);
		uiStorage.m_Storage.erase(sticky);
	};
	inline void ClearStickies(nabi::Context& context)
	{
		LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_UI << "Clearing all stickies from UIStorage!" << ENDLINE);

		SComp::UIStorageComponent& uiStorage = GetUIStorageComponent(context);
		uiStorage.m_Storage.clear();
	};

	// --- UI Input ---

	bool CheckIfMouseIsOverElement(nabi::Context const& context, dx::XMFLOAT2 const elementPosition, dx::XMFLOAT2 const elementDimensions);
	bool CheckIfMouseIsOverElement(nabi::Context const& context, dx::XMFLOAT2 const mousePosition, dx::XMFLOAT2 const elementPosition, dx::XMFLOAT2 const elementDimensions);
} // namespace ecs::UIModule

