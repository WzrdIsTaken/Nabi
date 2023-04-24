#pragma once
#include "Core.h"

#include "CoreSystems\UISystem.h"

#include "CoreModules\EntityModule.h"
#include "CoreModules\InputModule.h"
#include "CoreModules\ReflectionModule.h"
#include "CoreModules\UIModule.h"

namespace ecs
{
	REFLECT_SYSTEM_BEGIN_DEFAULT(UISystem)
	RELFECT_SYSTEM_END(UISystem)

	UISystem::UISystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId)
		: SystemBase(context, systemId, systemGroupId)
	{
		m_Context.m_Registry.on_construct<UISceneComponent>().connect<&UISystem::OnUISceneCreated>(this);
		m_Context.m_Registry.on_destroy<UISceneComponent>().connect<&UISystem::OnUISceneDestroyed>(this);
	}

	UISystem::~UISystem()
	{
		m_Context.m_Registry.on_construct<UISceneComponent>().disconnect<&UISystem::OnUISceneCreated>(this);
		m_Context.m_Registry.on_destroy<UISceneComponent>().disconnect<&UISystem::OnUISceneDestroyed>(this);
	}

	void UISystem::Update()
	{
		// I think this system has to tick in order to handle console inputs, which aren't event driven ):
		// Unless I write an event system for that.. thats an option I guess..
		// Plus I don't like the fact we are branching in an update loop
		// But again, uni project - not infinite time

		UISceneComponent::UISceneEntities const* const uiEntities = UIModule::GetCurrentUISceneEntities(m_Context, UIModule::GetMode::FirstEnabled);

		if (uiEntities)
		{
			std::vector<SelectedUIElement> selectedUiElements = {};

			for (entt::entity const entity : *uiEntities)
			{
				// TODO - Currently this UI system just handles buttons. I think this is all it will ever need. 
				// If not, then some sort of switch will be needed here I think. Or, get a view of all the ui components and filter them by the current ui scene.
				// Could use syntax like this, and then check which component isn't null: 
				// auto [button, slider] = m_Context.m_Registry.try_get<ButtonComponent, SliderComponent>(entity);

				UIElementComponentBase& uiElement = m_Context.m_Registry.get<ButtonComponent>(entity); 

				if (uiElement.m_Interactable && uiElement.m_Active)
				{
					using namespace nabi::Input;

					InputInfo const inputInfo =
					{
						uiElement.m_InputType,
						uiElement.m_ActivationKey,
						uiElement.m_Controller
					};
					InputInfo const altInputInfo =
					{
						uiElement.m_AltInputType,
						uiElement.m_AltActivationKey,
						uiElement.m_Controller
					};

					InputState const inputState = GetInput(inputInfo, altInputInfo);

					if (inputState == InputState::Pressed)
					{
						SelectedUIElement const selectedUIElement = 
						{
							entity,
							uiElement
						};

						selectedUiElements.emplace_back(selectedUIElement);
						uiElement.m_Selected = true;
					}
				}
			}

			if (!selectedUiElements.empty())
			{
				for (SelectedUIElement const& uiElement : selectedUiElements)
				{
					entt::entity const uiEntity = uiElement.m_Entity;
					UIElementComponentBase& uiElementComponent = uiElement.m_Element.get();

					uiElementComponent.m_Selected = false;

					ReflectionModule::Constraints constexpr* const constraints = nullptr;
					ReflectionModule::CallReflectedFunction(uiElementComponent.m_Scene, uiElementComponent.m_Action, constraints, entt::forward_as_meta(m_Context), uiEntity);
				}
			}
		}
	}

	nabi::Input::InputState UISystem::GetInput(InputInfo const inputInfo, InputInfo const altInputInfo) const
	{
		using namespace nabi::Input;
		auto getInputHelper = [&](InputInfo const info) -> InputState
		{
			InputState inputState = InputState::Invalid;

			switch (info.m_InputType)
			{
				case InputType::Keyboard:
					inputState = InputModule::GetKeyboardKey(m_Context, info.m_InputCode);
					break;
				case InputType::Mouse:
					inputState = InputModule::GetMouseButton(m_Context, info.m_InputCode);
					break;
				case InputType::Controller:
					inputState = InputModule::GetControllerButton(m_Context, info.m_Controller, info.m_InputCode);
					break;
				default:
					ASSERT_FAIL("Using an unexpected InputType!");
					break;
			}

			return inputState;
		};

		InputState inputState = InputState::Invalid;
		inputState = getInputHelper(inputInfo);

		if (inputState == InputState::Invalid || inputState == InputState::Up) // | this expression?
		{
			inputState = getInputHelper(altInputInfo);
		}

		return inputState;
	}

	void UISystem::OnUISceneCreated(entt::registry& /*registry*/, entt::entity entity) const
	{
	    // this func is kinda expensive. can we optimise it?

		UISceneComponent& uiSceneComponent = m_Context.m_Registry.get<UISceneComponent>(entity);
		std::string_view const uiSceneEntityNames = uiSceneComponent.m_UISceneEntityNames.data();

		char constexpr delimiter = ',';
		std::vector<std::string_view> const uiSceneEntityNamesVec = nabi::Utils::StringUtils::SplitString(uiSceneEntityNames, delimiter);

		uiSceneComponent.m_UISceneEntities.reserve(uiSceneEntityNamesVec.size());

		for (std::string_view const uiSceneEntityName : uiSceneEntityNamesVec)
		{
			std::string trimedName = std::string(uiSceneEntityName.data(), uiSceneEntityName.length());
			nabi::Utils::StringUtils::Trim(trimedName);

			entt::hashed_string const uiSceneEntityNameHash = entt::hashed_string(trimedName.c_str());
			entt::entity const uiEntity = EntityModule::FindFirstEntityByName(m_Context, uiSceneEntityNameHash);

			if (uiEntity != entt::null)
			{
				uiSceneComponent.m_UISceneEntities.push_back(uiEntity);
			}
		}

		UIStateComponent& uiStateComponent = UIModule::GetUIStateComponent(m_Context);
		uiStateComponent.m_UIScenes.push(uiSceneComponent);
	}

	void UISystem::OnUISceneDestroyed(entt::registry& /*registry*/, entt::entity entity) const
	{
		// This uses try get because otherwise when the program exits we get exit code 3 on shutdown.
		// I guess because the reference is invalid as the UIStateComponent has already been deleted.

		UISceneComponent const* const uiSceneComponent = m_Context.m_Registry.try_get<UISceneComponent>(entity);
		UIStateComponent* const uiStateComponent = UIModule::TryGetUIStateComponent(m_Context);
		if (uiSceneComponent && uiStateComponent)
		{
			std::stack<UISceneComponent>& uiScenes = uiStateComponent->m_UIScenes;
			if (!uiScenes.empty())
			{
				if (uiScenes.top() == *uiSceneComponent)
				{
					uiScenes.pop();

					// Destroy the ui entities in the scene
					UISceneComponent::UISceneEntities uiElements = uiSceneComponent->m_UISceneEntities;
					std::for_each(uiElements.begin(), uiElements.end(),
						[&](entt::entity const entity)
						{
							m_Context.m_Registry.destroy(entity);
						});
				}
				else
				{
					ASSERT_FAIL("Trying to pop a ui scene which isn't at the top of the stack!");
				}
			}
			else
			{
				ASSERT_FAIL("I don't know how this happened...");
			}
		}
	}
} // namespace ecs::System
