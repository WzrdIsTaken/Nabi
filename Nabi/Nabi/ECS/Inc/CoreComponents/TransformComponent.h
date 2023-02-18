#pragma once
#include "Core.h"

namespace ecs
{
	struct TransformComponent : public nabi::ECS::ComponentBase
	{
		dx::XMFLOAT3 m_Position;
		dx::XMFLOAT3 m_Rotation;
		dx::XMFLOAT3 m_Scale;
	};
} // namespace ecs
