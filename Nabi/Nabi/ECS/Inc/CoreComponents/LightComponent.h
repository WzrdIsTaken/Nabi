#pragma once
#include "Core.h"

namespace ecs
{
	struct LightingProperties abstract
	{
		float m_AmbientIntensity;
		float m_DiffuseIntensity;
		float m_SpecularAttenuation;
		float m_SpecularIntensity;
	};

	struct DirectionalLightComponent final : public nabi::ECS::ComponentBase, 
		public LightingProperties
	{
		dx::XMFLOAT3 m_Direction;
		dx::XMFLOAT3 m_Colour; // float4?
		float m_Intensity;
	};

	struct SpotLightComponent final : public nabi::ECS::ComponentBase, 
		public LightingProperties
	{
	};

	struct PointLightComponent final : public nabi::ECS::ComponentBase, 
		public LightingProperties
	{
	};
} // namespace ecs
