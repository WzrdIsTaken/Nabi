#pragma once

#include "../../Core.h"

#include <string>

#include "../../ECS/ComponentBase.h"
#include "MockCustomDataType.h"

#ifdef _DEBUG

namespace nabitest::MockComponent
{
	// Settings
	struct MockComponentSettings : public nabi::ECS::ComponentSettingsBase
	{
		int m_IntType;
		float m_FloatType;
		std::string m_StringType;
		MockCustomDataType m_CustomType;
	};

	// Component
	struct MockComponent : public nabi::ECS::ComponentBase
	{
		int m_IntType;
		float m_FloatType;
		std::string m_StringType;
		MockCustomDataType m_CustomType;
		
		MockComponent()
			: m_IntType(0)
			, m_FloatType(0.0f)
			, m_StringType("")
			, m_CustomType({})
		{
		}

		explicit MockComponent(MockComponentSettings const& settings)
			: m_IntType(settings.m_IntType)
			, m_FloatType(settings.m_FloatType)
			, m_StringType(settings.m_StringType)
			, m_CustomType(settings.m_CustomType)
		{
		}
	};

	// Default settings
	MockComponentSettings const mockComponentDefaultSettings
	{
		.m_IntType = 5,
		.m_FloatType = 20.6f,
		.m_StringType = "I wrote this on 20/11/2022",
		.m_CustomType = { 6.4, true }
	}; // Transform Component Default Settings

	// Helper
	inline MockComponentSettings ConstructSettingsFromComponent(MockComponent const& component)
	{
		MockComponentSettings settings = mockComponentDefaultSettings;
		settings.m_IntType = component.m_IntType;
		settings.m_FloatType = component.m_FloatType;
		settings.m_StringType = component.m_StringType;
		settings.m_CustomType = component.m_CustomType;

		return settings;
	}

	REFLECT_COMPONENT_BEGIN(MockComponent, "MockComponent")
		REFLECT_COMPONENT_PROPERTY(MockComponent::m_IntType, "IntType")
		REFLECT_COMPONENT_PROPERTY(MockComponent::m_FloatType, "FloatType")
		REFLECT_COMPONENT_PROPERTY(MockComponent::m_StringType, "StringType")
		REFLECT_COMPONENT_PROPERTY(MockComponent::m_CustomType, "CustomType")
	REFLECT_COMPONENT_END()
} // namespace nabitest::MockComponent

#endif // #ifdef _DEBUG
