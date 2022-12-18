#pragma once

#include "../../Core.h"

#include <string>

#include "../../ECS/ComponentBase.h"
#include "MockCustomDataType.h"

#ifdef _DEBUG

namespace nabitest::ECS
{
	// Settings
	struct MockComponentSettings final : public nabi::ECS::ComponentSettingsBase
	{
		int m_IntType;
		float m_FloatType;
		std::string m_StringType;
		MockCustomDataType m_CustomType;
	};

	// Default settings
	MockComponentSettings const mockComponentDefaultSettings
	{
		.m_IntType = 5,
		.m_FloatType = 20.6f,
		.m_StringType = "I wrote this on 20/11/2022",
		.m_CustomType = { 6.4, true }
	};

	// Component
	struct MockComponent final : public nabi::ECS::ComponentBase
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
	
	REFLECT_COMPONENT_BEGIN(MockComponent, "MockComponent")
		REFLECT_COMPONENT_PROPERTY(MockComponent::m_IntType, "IntType")
		REFLECT_COMPONENT_PROPERTY(MockComponent::m_FloatType, "FloatType")
		REFLECT_COMPONENT_PROPERTY(MockComponent::m_StringType, "StringType")
		REFLECT_COMPONENT_PROPERTY(MockComponent::m_CustomType, "CustomType")
	REFLECT_COMPONENT_END()
} // namespace nabitest::ECS

#endif // #ifdef _DEBUG