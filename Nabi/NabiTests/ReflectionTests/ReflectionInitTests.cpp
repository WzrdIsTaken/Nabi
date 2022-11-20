#include "CppUnitTest.h"

#include "Core/NabiCore.h"
#include "ECS/CoreComponents/EntityInfoComponent.h"
#include "Reflection/MetaObjectLookup.h"
#include "Reflection/XmlParser.h"

#include "../MockObjects/MockComponent.h"
#include "../MockObjects/MockCustomDataType.h"
#include "../MockObjects/MockGameCore.h"
#include "../MockObjects/MockSystem.h"
#include "../Utils/TestHelpers.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// Tests the process of passing an xml and creating a system and component

namespace nabitests::ReflectionTests
{
	TEST_CLASS(ReflectionInitTests)
	{
	public:
		TEST_METHOD(ParseSystems)
		{
			// Mock objects
			MockGameCore mockGameCore{};
			std::string const mockRouteDocPath = "TestData/TestReflectionData/test_route_file.xml";

			// Deserialize data files
			nabi::Reflection::MetaObjectLookup systemsLookup{};
			nabi::Reflection::XmlParser xmlParser{};

			entt::registry& registry = mockGameCore.GetRegistry();
			xmlParser.ParseXml(mockRouteDocPath, registry, &systemsLookup);

			// Get the system
			MockSystem::MockSystem mockSystem = systemsLookup.GetObject<MockSystem::MockSystem>("MockSystem");

			// Test that the system data is what we expect
			Comparison<int> systemDataComparison;
			systemDataComparison.m_Expected = 5;
			systemDataComparison.m_Actual = mockSystem.GetMockSystemData();

			Assert::AreEqual(systemDataComparison.m_Expected, systemDataComparison.m_Actual);
		}

		TEST_METHOD(ParseComponents)
		{
			/*
			// Mock objects
			MockGameCore mockGameCore{};
			std::string const mockRouteDocPath = "route.xml";

			// Deserialize data files
			nabi::Reflection::XmlParser xmlParser{};

			entt::registry& registry = mockGameCore.GetRegistry();
			xmlParser.ParseXml(mockRouteDocPath, registry, nullptr);

			// Check that this is only one entity in the registry
			Comparison<size_t> numberOfEntities(1, registry.alive());
			Assert::AreEqual(numberOfEntities.m_Expected, numberOfEntities.m_Actual);

			// Iterate over the registery and check for MockComponent
			auto mockComponentView = registry.view<MockComponent::MockComponent>();

			Comparison<int> intType(100);
			Comparison<float> floatType(50.8f);
			Comparison<std::string> stringType("somestring");
			Comparison<MockCustomDataType> customType({ 30.4, true });

			for (auto [entity, mockComponent] : mockComponentView.each())
			{
				intType.m_Actual = mockComponent.m_IntType;
				floatType.m_Actual = mockComponent.m_FloatType;
				stringType.m_Actual = mockComponent.m_StringType;
				customType.m_Actual = mockComponent.m_CustomType;
			}

			Assert::AreEqual(intType.m_Expected, intType.m_Actual);
			Assert::AreEqual(floatType.m_Expected, floatType.m_Actual);
			Assert::AreEqual(stringType.m_Expected, stringType.m_Actual);
			Assert::AreEqual(customType.m_Expected, customType.m_Actual);

			// Iterate over the registery and check for EntityInfoComponent
			auto entityInfoComponentView = registry.view<EntityInfoComponent::EntityInfoComponent>();

			Comparison<entt::hashed_string> entityGroup("TestGroup"_hs);
			Comparison<entt::hashed_string> entityName("TestName"_hs);

			for (auto [entity, entityInfoComponent] : entityInfoComponentView.each())
			{
				entityGroup.m_Actual = entityInfoComponent.m_EntityGroup;
				entityName.m_Actual = entityInfoComponent.m_EntityName;
			}

			Assert::AreEqual(entityGroup.m_Expected, entityGroup.m_Actual);
			Assert::AreEqual(entityName.m_Expected, entityName.m_Actual);
			*/
		}
	};
} // namespace nabitests::ReflectionTests
