#pragma once
#include "Core.h"

namespace ecs
{
	struct TransformComponent final : public nabi::ECS::ComponentBase
	{
		dx::XMFLOAT3 m_Position;
		dx::XMFLOAT3 m_Rotation; // Stored as euler, converted to quanterion in RenderSystem / wherever else its needed
		dx::XMFLOAT3 m_Scale;
	};
} // namespace ecs
