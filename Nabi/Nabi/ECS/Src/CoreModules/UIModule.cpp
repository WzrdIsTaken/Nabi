#include "Core.h"

#include "CoreModules\UIModule.h"

#include "CoreModules\InputModule.h"

namespace ecs::UIModule
{
	UISceneComponent::UISceneEntities const* const GetCurrentUISceneEntities(nabi::Context const& context, GetMode const getMode)
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

	bool CheckIfMouseIsOverElement(nabi::Context const& context, dx::XMFLOAT2 const elementPosition, dx::XMFLOAT2 const elementDimensions)
	{
		dx::XMFLOAT2 const mousePosition = InputModule::GetMousePosition(context);
		return CheckIfMouseIsOverElement(context, mousePosition, elementPosition, elementDimensions);
	}

	bool CheckIfMouseIsOverElement(nabi::Context const& context, dx::XMFLOAT2 const mousePosition, dx::XMFLOAT2 const elementPosition, dx::XMFLOAT2 const elementDimensions)
	{
		bool mouseOverElement = true;

		mouseOverElement &= mousePosition.x > elementPosition.x;
		mouseOverElement &= mousePosition.y > elementPosition.y;
		mouseOverElement &= mousePosition.x < elementPosition.x + elementDimensions.x;
		mouseOverElement &= mousePosition.y < elementPosition.y + elementDimensions.y;

		return mouseOverElement;
	}
} // namespace ecs::UIModule
