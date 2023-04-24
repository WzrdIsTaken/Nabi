#include "Core.h"

#include "CoreComponents\UIComponents.h"

// This is because if more ui components are added, it would get tedious to reflect all the base members again
#define REFLECT_UI_COMPONENT_BASE_PROPERTIES(uiComponentType) \
	REFLECT_COMPONENT_PROPERTY(CONCAT(uiComponentType, ::m_Scene), "Scene") \
	REFLECT_COMPONENT_PROPERTY(CONCAT(uiComponentType, ::m_Action), "Action") \
	REFLECT_COMPONENT_PROPERTY(CONCAT(uiComponentType, ::m_InputType), "InputType") \
	REFLECT_COMPONENT_PROPERTY(CONCAT(uiComponentType, ::m_ActivationKey), "ActivationKey") \
	REFLECT_COMPONENT_PROPERTY(CONCAT(uiComponentType, ::m_AltInputType), "AltInputType") \
	REFLECT_COMPONENT_PROPERTY(CONCAT(uiComponentType, ::m_AltActivationKey), "AltActivationKey") \
	REFLECT_COMPONENT_PROPERTY(CONCAT(uiComponentType, ::m_Controller), "Controller") \
	REFLECT_COMPONENT_PROPERTY(CONCAT(uiComponentType, ::m_Interactable), "Interactable") \
	REFLECT_COMPONENT_PROPERTY(CONCAT(uiComponentType, ::m_Active), "Active") \
	REFLECT_COMPONENT_PROPERTY(CONCAT(uiComponentType, ::m_Selected), "StartSelected")

namespace ecs
{
	REFLECT_COMPONENT_BEGIN_DEFAULT(UISceneComponent)
		REFLECT_COMPONENT_PROPERTY(UISceneComponent::m_SceneName, "SceneName")
		REFLECT_COMPONENT_PROPERTY(UISceneComponent::m_UISceneEntityNames, "SceneEntities")
		REFLECT_COMPONENT_PROPERTY(UISceneComponent::m_Enabled, "Enabled")
	REFLECT_COMPONENT_END(UISceneComponent)

	REFLECT_COMPONENT_BEGIN_DEFAULT(ButtonComponent)
		REFLECT_UI_COMPONENT_BASE_PROPERTIES(ButtonComponent)
	REFLECT_COMPONENT_END(ButtonComponent)
} // namespace ecs

#undef REFLECT_UI_COMPONENT_BASE_PROPERTIES
