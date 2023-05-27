#pragma once
#include "Core.h"

#include "InputCodes.h"

namespace ecs
{
	// 1 entity per scene has this component
	struct UISceneComponent final
	{
		//typedef std::vector<entt::hashed_string> UISceneEntityNames; (i realised this could just be a string - will save me a lot of headaches)
		typedef std::vector<entt::entity> UISceneEntities;

		// Authoring
		entt::hashed_string m_SceneName;
		entt::hashed_string m_UISceneEntityNames;
		bool m_Enabled;

		// Internal
		UISceneEntities m_UISceneEntities;

		UISceneComponent()
			: m_SceneName("InvalidScene"_hs)
			, m_UISceneEntityNames("InvalidNames"_hs)
			, m_UISceneEntities({})
			, m_Enabled(true)
		{
		}

		constexpr bool operator == (UISceneComponent const& other) const
		{
			return m_SceneName == other.m_SceneName &&
				   m_UISceneEntities == other.m_UISceneEntities;
		}
	};

#pragma warning( push )
#pragma warning( disable : 26495 ) // [var] is unitialized

	struct UIElementComponentBase abstract
	{
		entt::hashed_string m_Scene;
		entt::hashed_string m_Action;

		nabi::Input::InputType m_InputType;
		nabi::Input::InputCode m_ActivationKey; 
		
		nabi::Input::InputType m_AltInputType;
		nabi::Input::InputCode m_AltActivationKey;

		nabi::Input::Controller m_Controller; // Only set if you're, eg, making a 2 player game. Defaults to master
		
		bool m_Interactable;
		bool m_Active; // can be used for eg, setting if a button is hovered
		bool m_Selected;

		UIElementComponentBase()
			: m_Scene(nabi::Reflection::ReflectionGlobals::c_InvalidType)
			, m_Action(nabi::Reflection::ReflectionGlobals::c_InvalidFunction)
			, m_InputType(nabi::Input::InputType::Invalid)
			, m_ActivationKey(nabi::Input::InputCode::Invalid)
			, m_AltInputType(nabi::Input::InputType::Invalid)
			, m_AltActivationKey(nabi::Input::InputCode::Invalid)
			, m_Controller(nabi::Input::Controller::MasterController)
			, m_Interactable(true)
			, m_Active(true) // defaulted to true because if their is no special button behavior we still want to detect inputs from it
			, m_Selected(false)
		{
		}
	};

#pragma warning( pop )

	struct ButtonComponent final : public UIElementComponentBase
	{
		using UIElementComponentBase::UIElementComponentBase;
	};

	// Eg in the future might have a slider with slider% member
	// Probs not though...
	// NOTE - If more ui components are added, UISystem needs to be updated (see the 'UIElementComponentBase& uiElement = m_Context.m_Registry.get<ButtonComponent>(entity);' line)
} // namespace ecs
