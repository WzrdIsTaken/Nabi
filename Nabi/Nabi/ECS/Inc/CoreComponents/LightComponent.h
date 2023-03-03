#pragma once
#include "Core.h"

namespace ecs
{
	struct DirectionalLightComponent final : public nabi::ECS::ComponentBase
	{
		dx::XMFLOAT3 m_Direction;
		dx::XMFLOAT3 m_Colour; // float4?
		float m_Intensity;
	};

	struct SpotLightComponent final : public nabi::ECS::ComponentBase
	{
	};

	struct PointLightComponent final : public nabi::ECS::ComponentBase
	{
	};
} // namespace ecs
