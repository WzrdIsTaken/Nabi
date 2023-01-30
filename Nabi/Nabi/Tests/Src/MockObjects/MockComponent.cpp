#include "Core.h"

#include "MockObjects/MockComponent.h"

#ifdef RUN_TESTS

namespace nabitest::ECS
{
	REFLECT_COMPONENT_BEGIN(MockComponent, "MockComponent")
		REFLECT_COMPONENT_PROPERTY(MockComponent::m_IntType, "IntType")
		REFLECT_COMPONENT_PROPERTY(MockComponent::m_FloatType, "FloatType")
		REFLECT_COMPONENT_PROPERTY(MockComponent::m_StringType, "StringType")
		REFLECT_COMPONENT_PROPERTY(MockComponent::m_CustomType, "CustomType")
		REFLECT_COMPONENT_PROPERTY(MockComponent::m_Enum, "Enum")
	REFLECT_COMPONENT_END(MockComponent)
} // namespace nabitest::ECS

#endif // #ifdef RUN_TESTS
