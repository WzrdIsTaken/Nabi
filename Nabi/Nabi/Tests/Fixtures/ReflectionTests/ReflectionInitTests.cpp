#include "../../TestCore.h"

#include "../../../ECS/CoreComponents/EntityInfoComponent.h"
#include "../../../Reflection/MetaObjectLookup.h"
#include "../../../Reflection/XmlParser.h"

#ifdef _DEBUG

namespace nabitest::ReflectionTests
{
	// Check system files can be loaded correctly
	TEST(RelfectionTests, ParseSystemsFromRoute)
	{
		// Mock objects
		entt::registry registry;
		std::string const routeDocPath = "Tests/TestData/TestReflectionData/test_route_file.xml";

		// Deserialize data files
		nabi::Reflection::MetaObjectLookup systemsLookup{};
		nabi::Reflection::XmlParser xmlParser{};

		xmlParser.ParseXml(routeDocPath, registry, &systemsLookup);

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
		entt::registry registry;
		std::string const routeDocPath = "Tests/TestData/TestReflectionData/test_route_file.xml";

		// Deserialize data files
		nabi::Reflection::XmlParser xmlParser{};
		xmlParser.ParseXml(routeDocPath, registry, nullptr);

		// Check that this is only one entity in the registry
		Comparison<size_t> numberOfEntities(1, registry.alive());
		COMPAIR_EQ(numberOfEntities);

		// Iterate over the registery and check for MockComponent
		auto mockComponentView = registry.view<MockComponent>();

		Comparison<int> intType(20);
		Comparison<float> floatType(50.8f);
		Comparison<std::string> stringType("somestring");
		Comparison<MockCustomDataType> customType({ 27.3, true });

		for (auto [entity, mockComponent] : mockComponentView.each())
		{
			intType.m_Actual = mockComponent.m_IntType;
			floatType.m_Actual = mockComponent.m_FloatType;
			stringType.m_Actual = mockComponent.m_StringType;
			customType.m_Actual = mockComponent.m_CustomType;
		}

		COMPAIR_EQ(intType);
		COMPAIR_EQ(floatType);
		COMPAIR_EQ(stringType);
		COMPAIR_EQ(customType);

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

#endif // #ifdef _DEBUG