#include "../TestCore.h"

#include "../../ECS/CoreComponents/EntityInfoComponent.h"
#include "../../Reflection/MetaObjectLookup.h"
#include "../../Reflection/XmlParser.h"

#ifdef _DEBUG

namespace nabitest::ReflectionTests
{
	// Check system files can be loaded correctly
	TEST(RelfectionTests, ParseSystems)
	{
		// Mock objects
		entt::registry registry;
		std::string const mockRouteDocPath = "Tests/TestData/TestReflectionData/test_route_file.xml";

		// Deserialize data files
		nabi::Reflection::MetaObjectLookup systemsLookup{};
		nabi::Reflection::XmlParser xmlParser{};

		xmlParser.ParseXml(mockRouteDocPath, registry, &systemsLookup);

		// Get the system
		MockSystem::MockSystem mockSystem = systemsLookup.GetObject<MockSystem::MockSystem>("MockSystem");

		// Test that the system data is what we expect
		Comparison<int> systemDataComparison{};
		systemDataComparison.m_Expected = 5;
		systemDataComparison.m_Actual = mockSystem.GetMockSystemData();

		EXPECT_EQ(systemDataComparison.m_Expected, systemDataComparison.m_Actual);
	}

	// Check entity files can be loaded correctly
	TEST(RelfectionTests, ParseEntities)
	{
		// Mock objects
		entt::registry registry;
		std::string const mockRouteDocPath = "Tests/TestData/TestReflectionData/test_route_file.xml";

		// Deserialize data files
		nabi::Reflection::XmlParser xmlParser{};
		xmlParser.ParseXml(mockRouteDocPath, registry, nullptr);

		// Check that this is only one entity in the registry
		Comparison<size_t> numberOfEntities(1, registry.alive());
		EXPECT_EQ(numberOfEntities.m_Expected, numberOfEntities.m_Actual);

		// Iterate over the registery and check for MockComponent
		auto mockComponentView = registry.view<MockComponent::MockComponent>();

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

		EXPECT_EQ(intType.m_Expected, intType.m_Actual);
		EXPECT_EQ(floatType.m_Expected, floatType.m_Actual);
		EXPECT_EQ(stringType.m_Expected, stringType.m_Actual);
		EXPECT_EQ(customType.m_Expected, customType.m_Actual);

		// Iterate over the registery and check for EntityInfoComponent
		auto entityInfoComponentView = registry.view<EntityInfoComponent::EntityInfoComponent>();

		Comparison<entt::hashed_string> entityGroup("TestGroup"_hs);
		Comparison<entt::hashed_string> entityName("TestEntity"_hs);

		for (auto [entity, entityInfoComponent] : entityInfoComponentView.each())
		{
			entityGroup.m_Actual = entityInfoComponent.m_EntityGroup;
			entityName.m_Actual = entityInfoComponent.m_EntityName;
		}

		EXPECT_EQ(entityGroup.m_Expected, entityGroup.m_Actual);
		EXPECT_EQ(entityName.m_Expected, entityName.m_Actual);
	}
} // namespace nabitest::ReflectionTests

#endif // #ifdef _DEBUG
