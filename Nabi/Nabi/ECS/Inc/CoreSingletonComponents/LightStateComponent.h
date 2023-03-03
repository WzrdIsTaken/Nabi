#pragma once
#include "Core.h"

namespace ecs
{
	struct LightStateComponent final : public nabi::ECS::ComponentBase
	{
		int m_LightCount;
		bool m_UpdateLights;
		bool m_UpdateGlobalLightProperties;

		// Global light properties
		float m_AmbientIntensity;
		float m_DiffuseIntensity;
		float m_SpecularAttenuation;
		float m_SpecularIntensity;
	};
} // namespace ecs
