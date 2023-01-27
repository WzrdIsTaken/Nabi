#include "../../TestCore.h"

#include "../../../ECS/Inc/CoreComponents/EntityInfoComponent.h"
#include "../../../Reflection/Inc/EntityCreator.h"
#include "../../../Reflection/Inc/EntityPropertyList.h"

#ifdef RUN_TESTS

namespace nabitest::ReflectionTests
{
	// Check creating a runtime entity works as expected
	TEST(RelfectionTests, CreateEntityAtRuntime)
	{
		// --- Setup ---

		using namespace nabi::Reflection;

		// Mock objects
		ComponentData mockComponentData;
		mockComponentData.m_Id = "MockComponent"_hs; // Important that this matches up with the reflected MockObjects/MockComponent

		EntityTemplateData mockEntityTemplateData;
		mockEntityTemplateData.m_Id = "MockEntityTemplate"_hs; // This can be anything for testing
		mockEntityTemplateData.m_Components.emplace_back(mockComponentData);

		entt::registry registry;
		EntityCreator::EntityTemplateStore entityStore;

		std::pair<std::string, EntityTemplateData> entityTemplatePair(mockEntityTemplateData.m_Id.data(), mockEntityTemplateData);
		entityStore.emplace(entityTemplatePair);

		EntityCreator entityCreator(registry, std::move(entityStore));

		// --- Entity Creation ---

		// Create an entity! (fingers crossed...)
		EntityCreator::EntityCreationSettings entityCreationSettings1;
		entityCreationSettings1.m_EntityTemplateName = "MockEntityTemplate";
		entityCreator.CreateEntity(entityCreationSettings1);

		// Create an entity with a custom name
		EntityCreator::EntityCreationSettings entityCreationSettings2;
		entityCreationSettings2.m_EntityTemplateName = "MockEntityTemplate";
		entityCreationSettings2.m_EntityName = "EntityWithCustomName";
		entityCreationSettings2.m_AppendUUID = false;
		entityCreator.CreateEntity(entityCreationSettings2);

		// Create another entity, this time with a property override
		EntityCreator::EntityCreationSettings entityCreationSettings3;
		EntityPropertyList propertyList;
		propertyList.AddProperty("MockComponent", "IntType", "7"); // default value is 5

		entityCreationSettings3.m_EntityTemplateName = "MockEntityTemplate";
		entityCreationSettings3.m_EntityName = "EntityWithCustomNameAndOverriddenProperty";
		entityCreationSettings3.m_EntityOverriddenProperties = &propertyList;
		entityCreationSettings3.m_AppendUUID = false;

		entityCreator.CreateEntity(entityCreationSettings3);

		// --- Tests ---

		// Check that one entity exists in the store
		Comparison<size_t> entityStoreSizeComparison(1, entityCreator.GetEntityStoreSize());
		COMPAIR_EQ(entityStoreSizeComparison);

		// Check that there are 3 entities in the registery
		Comparison<size_t> entitiesInRegisteryComparison(3, registry.size());
		COMPAIR_EQ(entitiesInRegisteryComparison);

		// Check that entities have the correct values
		Comparison<int> entityDefaultCreatedHasDefaultIntValue(5);
		Comparison<bool> entityWithCustomNameIsFound(true);
		Comparison<int> entityWithOverridenPropertyHasUpdatedIntValue(7);

		auto const mockComponentView = registry.view<ecs::EntityInfoComponent, MockComponent>();

		for (auto const [entity, entityInfo, mockComponent] : mockComponentView.each())
		{
			switch (entityInfo.m_EntityName)
			{
				case "Entity_0"_hs: // Created entities with a UUID added have there entity ID appended. The default name for an entity is "Entity"
					entityDefaultCreatedHasDefaultIntValue.m_Actual = mockComponent.m_IntType;
					break;
				case "EntityWithCustomName"_hs:
					entityWithCustomNameIsFound.m_Actual = true;
					break;
				case "EntityWithCustomNameAndOverriddenProperty"_hs:
					entityWithOverridenPropertyHasUpdatedIntValue.m_Actual = mockComponent.m_IntType;
					break;
				default:
					// The entity could not be found
					ASSERT_EQ(false, true);
					break;
			}
		}

		COMPAIR_EQ(entityDefaultCreatedHasDefaultIntValue);
		COMPAIR_EQ(entityWithCustomNameIsFound);
		COMPAIR_EQ(entityWithOverridenPropertyHasUpdatedIntValue);
	}
} // namespace nabitest::ReflectionTests

#endif // #ifdef RUN_TESTS