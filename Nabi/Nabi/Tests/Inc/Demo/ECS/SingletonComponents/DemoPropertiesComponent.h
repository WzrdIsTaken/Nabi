#pragma once
#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "InputCodes.h"

namespace ecs::SComp
{
	struct DemoPropertiesComponent final : nabi::ECS::ComponentBase
	{
		nabi::Input::InputCode m_LoadAsteroidGroupKey;
		nabi::Input::InputCode m_UnloadAsteroidGroupKey;

		DemoPropertiesComponent()
			: m_LoadAsteroidGroupKey(nabi::Input::InputCode::Key_O)
			, m_UnloadAsteroidGroupKey(nabi::Input::InputCode::Key_P)
		{
		}
	};
} // namespace ecs::SComp

#endif // ifdef INCLUDE_DEMO
