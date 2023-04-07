#pragma once
#include "Core.h"

#include "CoreModules\InputModule.h"

namespace ecs::InputModule
{
	using namespace nabi::Input;

	nabi::Input::InputState GetKeyboardKey(nabi::Context const& context, int const key)
	{
		KeyboardState const& keyboardState = GetKeyboardState(context);
		InputState keyState = InputState::Invalid;

		if (keyboardState.m_CurrentKeyStates[key] && !keyboardState.m_PreviousKeyStates[key])
		{
			keyState = InputState::Pressed;
		}
		else if (!keyboardState.m_CurrentKeyStates[key] && keyboardState.m_PreviousKeyStates[key])
		{
			keyState = InputState::Released;
		}
		else if (keyboardState.m_CurrentKeyStates[key])
		{
			keyState = InputState::Held;
		}
		else if (!keyboardState.m_CurrentKeyStates[key])
		{
			keyState = InputState::Up;
		}

		return keyState;
	}

	nabi::Input::InputState GetMouseButton(nabi::Context const& context, int const button)
	{
		MouseState const& mouseState = GetMouseState(context);
		InputState buttonState = InputState::Invalid;

		if (mouseState.m_CurrentButtonStates[button] && !mouseState.m_PreviousButtonStates[button])
		{
			buttonState = InputState::Pressed;
		}
		else if (!mouseState.m_CurrentButtonStates[button] && mouseState.m_PreviousButtonStates[button])
		{
			buttonState = InputState::Released;
		}
		else if (mouseState.m_CurrentButtonStates[button])
		{
			buttonState = InputState::Held;
		}
		else if (!mouseState.m_CurrentButtonStates[button])
		{
			buttonState = InputState::Up;
		}

		return buttonState;
	}

	dx::XMFLOAT2 GetMousePosition(nabi::Context const& context)
	{
		return GetMouseState(context).m_MousePosition;
	}

	nabi::Input::InputState GetControllerButton(nabi::Context const& context, unsigned int const controller, int const button)
	{
		ControllerState const& controllerState = GetControllerState(context);
		InputState buttonState = InputState::Invalid;

		if      ((controllerState.m_CurrentControllerStates[controller].m_Buttons & button) != 0 &&
			    (controllerState.m_PreviousControllerStates[controller].m_Buttons & button) == 0)
		{
			buttonState = InputState::Pressed;
		}
		else if ((controllerState.m_CurrentControllerStates[controller].m_Buttons & button) == 0 &&
			     (controllerState.m_PreviousControllerStates[controller].m_Buttons & button) != 0)
		{
			buttonState = InputState::Released;
		}
		else if ((controllerState.m_CurrentControllerStates[controller].m_Buttons & button) != 0)
		{
			buttonState = InputState::Held;
		}
		else if ((controllerState.m_CurrentControllerStates[controller].m_Buttons & button) == 0)
		{
			buttonState = InputState::Up;
		}


		return buttonState;
	}

	float GetControllerAxis(nabi::Context const& context, unsigned int const controller, unsigned int const axis)
	{
		return GetControllerState(context).m_CurrentControllerStates[controller].m_Axis[axis];
	}

	bool GetControllerConnected(nabi::Context const& context, unsigned int const controller)
	{
		return GetControllerState(context).m_CurrentControllerStates[controller].m_Connected;
	}
} // namespace ecs
