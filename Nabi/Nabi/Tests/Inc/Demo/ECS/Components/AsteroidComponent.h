#pragma once
#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

namespace ecs
{
	struct AsteroidComponent final : public nabi::ECS::ComponentBase
	{
		dx::XMFLOAT3 m_SpinDirection;
		float m_SpinSpeed;

		AsteroidComponent()
			: m_SpinDirection{ 0.0f, 0.0f, 0.0f }
			, m_SpinSpeed(0.0f)
		{
		}
	};
} // namespace ecs

#endif // ifdef INCLUDE_DEMO
