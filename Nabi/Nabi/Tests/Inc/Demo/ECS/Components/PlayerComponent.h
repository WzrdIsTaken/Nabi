#pragma once
#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "InputCodes.h"

namespace ecs
{
	struct PlayerComponent final : public nabi::ECS::ComponentBase
	{
		nabi::Input::InputCode m_ForwardKey;
		nabi::Input::InputCode m_BackwardKey;
		nabi::Input::InputCode m_LeftKey;
		nabi::Input::InputCode m_RightKey;
		nabi::Input::InputCode m_UpKey;
		nabi::Input::InputCode m_DownKey;
		float m_Speed;

		PlayerComponent()
			: m_ForwardKey(nabi::Input::InputCode::Key_W)
			, m_BackwardKey(nabi::Input::InputCode::Key_S)
			, m_LeftKey(nabi::Input::InputCode::Key_A)
			, m_RightKey(nabi::Input::InputCode::Key_D)
			, m_UpKey(nabi::Input::InputCode::Key_Q)
			, m_DownKey(nabi::Input::InputCode::Key_E)
			, m_Speed(10.0f)
		{
		}
	};
} // namespace ecs

#endif // ifdef INCLUDE_DEMO
