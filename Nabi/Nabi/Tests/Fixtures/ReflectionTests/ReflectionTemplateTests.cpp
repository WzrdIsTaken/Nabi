#include "../../TestCore.h"

#include "../../../Libraries/pugixml/pugixml.hpp"

#include "../../../ECS/CoreComponents/EntityInfoComponent.h"
#include "../../../Reflection/MetaObjectLookup.h"
#include "../../../Reflection/XmlParser.h"

#ifdef _DEBUG

namespace nabitest::ReflectionTests
{
	// Check entity template inheritance works correctly
	TEST(RelfectionTests, ParseEntityTemplates)
	{
		// Mock objects
		entt::registry registry;
		std::string const docPath = "Tests/TestData/TestReflectionData/test_entity_template_file.xml";

		// Deserialize data files
		nabi::Reflection::MetaObjectLookup systemsLookup{};
		nabi::Reflection::XmlParser xmlParser{};

		pugi::xml_document const doc = xmlParser.LoadDocument(docPath);
		xmlParser.ParseEntities(doc, registry);

		// Check that this is only two entities in the registry
		Comparison<size_t> numberOfEntities(4, registry.alive());
		COMPAIR_EQ(numberOfEntities);

		// Iterate over the registery and get all of the entities. The following logic is very jank but hopefully clear
		auto mockComponentView = registry.view<ecs::EntityInfoComponent, MockComponent>();

		// Init the comparisons
		Comparison<int> entityOneIntType(27);
		Comparison<float> entityOneFloatType(1.0f);

		Comparison<float> entityTwoFloatType(2.0f);

		Comparison<int> entityThreeIntType(27);
		Comparison<std::string> entityThreeStringType("inheritance");

		Comparison<float> entityFourFloatType(3.0f);

		// Loop through all 4 entities, doing a simple switch and set the comparisons
		for (auto [entity, entityInfo, mockComponent] : mockComponentView.each())
		{
			switch (entityInfo.m_EntityName)
			{
				case "TestEntity1"_hs:
					entityOneIntType.m_Actual = mockComponent.m_IntType;
					entityOneFloatType.m_Actual = mockComponent.m_FloatType;
					break;
				case "TestEntity2"_hs:
					entityTwoFloatType.m_Actual = mockComponent.m_FloatType;
					break;
				case "TestEntity3"_hs:
					entityThreeIntType.m_Actual = mockComponent.m_IntType;
					entityThreeStringType.m_Actual = mockComponent.m_StringType;
					break;
				case "TestEntity4"_hs:
					entityFourFloatType.m_Actual = mockComponent.m_FloatType;
					break;
				default:
					ASSERT_FAIL("The entity " << WRAP(entityInfo.m_EntityName.data(), "'") << " is not used in ReflectionTests/ParseEntityTemplate!");
					break;
			}
		}

		// Run the asserts
		COMPAIR_EQ(entityOneIntType);
		COMPAIR_EQ(entityOneFloatType);

		COMPAIR_EQ(entityTwoFloatType);

		COMPAIR_EQ(entityThreeIntType);
		COMPAIR_EQ(entityThreeStringType);

		COMPAIR_EQ(entityFourFloatType);
	}
} // namespace nabitest::ReflectionTests

#endif // #ifdef _DEBUG
