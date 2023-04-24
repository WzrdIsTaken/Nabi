#pragma once
#include "Core.h"

#include "InputCodes.h"
#include "InputState.h"

namespace ecs
{
	struct UIElementComponentBase;
} // namespace ecs

namespace ecs
{
	class UISystem final : public nabi::ECS::SystemBase
	{
	public:
		UISystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId);
		~UISystem();

		void Update(/*TODO - game time?*/);

	private:
		struct SelectedUIElement
		{
			entt::entity m_Entity;
			std::reference_wrapper<UIElementComponentBase> m_Element;
		};
		struct InputInfo
		{
			nabi::Input::InputType m_InputType;
			nabi::Input::InputCode m_InputCode;
			nabi::Input::Controller m_Controller;
		};

		nabi::Input::InputState GetInput(InputInfo const inputInfo, InputInfo const altInputInfo) const;

		void OnUISceneCreated(entt::registry& registry, entt::entity entity) const;
		void OnUISceneDestroyed(entt::registry& registry, entt::entity entity) const;

		REFLECT_PRIVATES(UISystem)
	};
} // namespace ecs
