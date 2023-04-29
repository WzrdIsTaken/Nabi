#pragma once
#include "Core.h"

#include "InputCodes.h"
#include "InputState.h"
#include "CoreSingletonComponents\InputStateComponent.h"

namespace ecs::InputModule
{
	// These functions query the InputStateComponent to check key/mouse/button state
	nabi::Input::InputState GetKeyboardKey(nabi::Context const& context, nabi::Input::InputCode const keyCode);

	nabi::Input::InputState GetMouseButton(nabi::Context const& context, nabi::Input::InputCode const buttonCode);
	dx::XMFLOAT2 GetMousePosition(nabi::Context const& context);

	nabi::Input::InputState GetControllerButton(nabi::Context const& context, nabi::Input::Controller const controllerCode, nabi::Input::InputCode const buttonCode);
	float GetControllerAxis(nabi::Context const& context, nabi::Input::Controller const controllerCode, nabi::Input::InputCode const axis);
	bool GetControllerConnected(nabi::Context const& context, nabi::Input::Controller const controllerCode);

	// Input state getters
	inline SComp::InputStateComponent const& GetInputStateComponent(nabi::Context const& context) {
		return context.m_Registry.get<SComp::InputStateComponent>(context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Input));
	};
	inline SComp::KeyboardState const& GetKeyboardState(nabi::Context const& context)     { return GetInputStateComponent(context).m_KeyboardState;   }
	inline SComp::MouseState const& GetMouseState(nabi::Context const& context)           { return GetInputStateComponent(context).m_MouseState;      }
	inline SComp::ControllerState const& GetControllerState(nabi::Context const& context) { return GetInputStateComponent(context).m_ControllerState; }

	inline SComp::KeyboardState& GetKeyboardState(nabi::Context& context)     { return const_cast<SComp::KeyboardState&>(GetInputStateComponent(context).m_KeyboardState);     }
	inline SComp::MouseState& GetMouseState(nabi::Context& context)           { return const_cast<SComp::MouseState&>(GetInputStateComponent(context).m_MouseState);           }
	inline SComp::ControllerState& GetControllerState(nabi::Context& context) { return const_cast<SComp::ControllerState&>(GetInputStateComponent(context).m_ControllerState); }
} // namespace ecs
