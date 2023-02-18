#include "TestCore.h"

#include "Context.h"
#include "CoreComponents/EntityInfoComponent.h"
#include "MetaObjectLookup.h"
#include "XmlParser.h"

#ifdef RUN_TESTS

namespace nabitest::ReflectionTests
{
	// Check system files can be loaded correctly
	TEST(RelfectionTests, ParseSystemsFromRoute)
	{
		// Mock objects
		nabi::Context context;
		context.m_Registry = {};
		std::string const routeDocPath = "Tests/Data/Reflection/test_route_file.xml";

		// Deserialize data files
		nabi::Reflection::MetaObjectLookup systemsLookup{};
		nabi::Reflection::XmlParser xmlParser{};

		xmlParser.ParseXml(routeDocPath, context, &systemsLookup);

		// Get the system
		MockSystem mockSystem = std::move(systemsLookup.GetObject<MockSystem>("MockSystem"));

		// Test that the system data is what we expect
		Comparison<int> systemDataComparison{};
		systemDataComparison.m_Expected = 5;
		systemDataComparison.m_Actual = mockSystem.GetMockSystemData();

		COMPAIR_EQ(systemDataComparison);
	}

	// Check entity files can be loaded correctly
	TEST(RelfectionTests, ParseEntitiesFromRoute)
	{
		// Mock objects
		nabi::Context context;
		context.m_Registry = {};

		std::string const routeDocPath = "Tests/Data/Reflection/test_route_file.xml";
		entt::registry& registry = context.m_Registry;

		// Deserialize data files
		nabi::Reflection::XmlParser xmlParser{};
		xmlParser.ParseXml(routeDocPath, context, nullptr);

		// Check that this is only one entity in the registry
		Comparison<size_t> numberOfEntities(1, registry.alive());
		COMPAIR_EQ(numberOfEntities);

		// Iterate over the registery and check for MockComponent
		auto mockComponentView = registry.view<MockComponent>();

		Comparison<int> intType(5); // 5 is the default value, IntType is unassigned in the xml
		Comparison<float> floatType(50.8f);
		Comparison<std::string> stringType("somestring");
		Comparison<MockCustomDataType> customType({ 27.3, true });
		Comparison<MockEnum> enumType(MockEnum::AnotherEntry);

		for (auto [entity, mockComponent] : mockComponentView.each())
		{
			intType.m_Actual    = mockComponent.m_IntType;
			floatType.m_Actual  = mockComponent.m_FloatType;
			stringType.m_Actual = mockComponent.m_StringType;
			customType.m_Actual = mockComponent.m_CustomType;
			enumType.m_Actual   = mockComponent.m_Enum;
		}

		COMPAIR_EQ(intType);
		COMPAIR_EQ(floatType);
		COMPAIR_EQ(stringType);
		COMPAIR_EQ(customType);
		COMPAIR_EQ(enumType);

		// Iterate over the registery and check for EntityInfoComponent
		auto entityInfoComponentView = registry.view<ecs::EntityInfoComponent>();

		Comparison<entt::hashed_string> entityGroup("TestGroup"_hs);
		Comparison<entt::hashed_string> entityName("TestEntity"_hs);

		for (auto [entity, entityInfoComponent] : entityInfoComponentView.each())
		{
			entityGroup.m_Actual = entityInfoComponent.m_EntityGroup;
			entityName.m_Actual = entityInfoComponent.m_EntityName;
		}

		COMPAIR_EQ(entityGroup);
		COMPAIR_EQ(entityName);
	}
} // namespace nabitest::ReflectionTests

#endif // #ifdef RUN_TESTS
