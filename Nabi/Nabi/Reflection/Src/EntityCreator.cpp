#include "EngineCore.h"

#include "EntityCreator.h"

#include "CoreComponents/EntityInfoComponent.h"
#include "CoreComponents/SpatialHierarchyComponent.h"
#include "DebugUtils.h"
#include "ECSUtils.h"
#include "EntityPropertyList.h"
#include "ReflectionGlobals.h"
#include "ReflectionHelpers.h"
#include "StringConverter.h"

namespace nabi::Reflection
{
	// --- Free Functions (used in EntityCreator and XmlParser) ---

	namespace Creation
	{
		void OverrideEntityComponents(std::vector<ComponentData>& componentData,
			entt::hashed_string const propertyComponentRef, entt::hashed_string const propertyId, entt::hashed_string const propertyValue,
			PropertyData const* const propertyDataOverride) NABI_NOEXCEPT
		{
			// Loop through all of the component's properties
			for (ComponentData& componentData : componentData)
			{
				// If the property is referencing this component
				if (componentData.m_Id == propertyComponentRef)
				{
					// Foreach property on that component
					for (PropertyData& propertyData : componentData.m_Properties)
					{
						// If we find the property on one of the component data, override it
						if (propertyData.m_Id == propertyId)
						{
							propertyData.m_Value = propertyValue;
							goto OverrideEntityComponentsExit;
						}
					}

					// If we didn't find it, but the property links to this component, it just means that the property has not been set in xml
					// and will be default initialized. To override that default initialization, add the property data to the component data
					if (propertyDataOverride == nullptr)
					{
						// If there is no set override, create a new one
						PropertyData additionalPropertyData;
						additionalPropertyData.m_Id = propertyId;
						additionalPropertyData.m_Value = propertyValue;

						componentData.m_Properties.push_back(additionalPropertyData);
					}
					else
					{
						// Else we can save the construction and just add the one which already exists
						componentData.m_Properties.emplace_back(*propertyDataOverride);
					}

					goto OverrideEntityComponentsExit;
				}
			}

			ASSERT_FAIL("The property is referencing a component which doesn't exist!");

		OverrideEntityComponentsExit:
#ifdef USE_DEBUG_UTILS
			if (g_XmlParserLog)
			{
				LOG(LOG_PREP, LOG_INFO, SPACE(INDENT_2) << LOG_CATEGORY_REFLECTION << "Found a property node with id " << WRAP(propertyId.data(), "'") 
					<< " overriding a value on " << WRAP(propertyComponentRef.data(), "'") << " with value " << WRAP(propertyValue.data(), "'") << ENDLINE);
			}
#endif // ifdef USE_DEBUG_UTILS
			return; // Because without the preprocessor block code, this would be a lable then end of function
		}

		void ResolveEntityComponents(std::vector<ComponentData> const& entityComponents, entt::registry& registery, entt::entity const entity) NABI_NOEXCEPT
		{
			// Spin through the entity components, resolve them and add all them all to the registery
			for (ComponentData const& component : entityComponents)
			{
				// Get the component's ID and resolve it
				std::string_view const componentId = component.m_Id.data();
				entt::meta_any metaComponent = ResolveComponent(component, componentId);

#ifdef USE_DEBUG_UTILS
				if (g_XmlParserLog)
				{
					LOG(LOG_PREP, LOG_INFO, SPACE(INDENT_2) << LOG_CATEGORY_REFLECTION << "Found a component on the entity with id " << WRAP(componentId, "'") << ENDLINE);
				}
#endif // ifdef USE_DEBUG_UTILS

				// Assign the component to the registery
				AssignComponentToRegistery(metaComponent, registery, entity);
			}
		}

		entt::meta_any ResolveComponent(ComponentData const& componentData, std::string_view const componentId) NABI_NOEXCEPT
		{
			// Resolve the component
			entt::meta_any metaComponent = ReflectionHelpers::ConstructMetaObject(componentId);
			
			// Iterate through all of the component's nodes children - these will be the component's properties 
			ResolveProperties(componentData, metaComponent);

			// Return the created component
			return metaComponent;
		}

		void ResolveProperties(MetaECSTypeData const& data, entt::meta_any& metaObject) NABI_NOEXCEPT
		{
			// Spin through all the system/components properties
			for (PropertyData const& propertyNode : data.m_Properties)
			{
				// Get the property's infomation
				std::string_view const propertyId = propertyNode.m_Id.data();
				std::string_view const propertyValue = propertyNode.m_Value.data();
				
#ifdef USE_DEBUG_UTILS
				if (g_XmlParserLog)
				{
					LOG(LOG_PREP, LOG_INFO, SPACE(INDENT_2) << LOG_CATEGORY_REFLECTION << "Found a property with id " 
						<< WRAP(propertyId, "'") << " and value " << WRAP(propertyValue, "'") << ENDLINE);
				}
#endif // ifdef USE_DEBUG_UTILS

				// Resolve the property
				ResolveProperty(metaObject, propertyId, propertyValue);
			}
		}

		void ResolveProperty(entt::meta_any& metaObject, std::string_view const propertyName, std::string_view const propertyValue) NABI_NOEXCEPT
		{
			// Get the properties name hash and data
			entt::hashed_string const metaPropertyHash = entt::hashed_string(propertyName.data());
			entt::meta_data const metaPropertyData = metaObject.type().data(metaPropertyHash);

			// If you get an error on this next line, double check that all your property names in the xml are correct!
			std::string_view const propertyTypeName = metaPropertyData.type().info().name();
			std::string propertyTypeNameAsStr = std::string(propertyTypeName);

			// Get the property's type hash
			StringConverter::ExtractTypeName(propertyTypeNameAsStr);
			entt::hashed_string const propertyTypeHash = entt::hashed_string(propertyTypeNameAsStr.c_str());

			// Convert the property from string, and assign it to the meta object's data
			StringConverter::ConvertFromString(propertyValue.data(), propertyTypeHash, metaObject, metaPropertyData);
		}

		void AddEntityInfoComponentToEntity(entt::registry& registey, entt::entity const entity,
			entt::hashed_string const& entityGroupHash, std::string_view entityName) NABI_NOEXCEPT
		{
			auto entityInfoComponentSettings = ecs::entityInfoComponentDefaultSettings;
			entityInfoComponentSettings.m_EntityGroup = entityGroupHash;
			entityInfoComponentSettings.m_EntityName = entt::hashed_string(entityName.data());

			registey.emplace<ecs::EntityInfoComponent>(entity, entityInfoComponentSettings);
		}

		void nabi::Reflection::Creation::AddSpatialHierarchyComponentToEntity(entt::registry& registey, entt::entity const parent) NABI_NOEXCEPT
		{
			auto spatialHierarchyComponentSettings = ecs::spatialHierarchyComponentDefaultSettings;
			spatialHierarchyComponentSettings.m_Parent = parent;
			spatialHierarchyComponentSettings.m_Children = {};

			registey.emplace<ecs::SpatialHierarchyComponent>(parent, spatialHierarchyComponentSettings);
		}

		void AssignComponentToRegistery(entt::meta_any& metaComponent, entt::registry& registery, entt::entity const entity) NABI_NOEXCEPT
		{
			// Find the assign function and call it on the specified entity
			if (entt::meta_func const assign = metaComponent.type().func(ReflectionGlobals::c_AssignFunctionName))
			{
				assign.invoke(metaComponent, entt::forward_as_meta(metaComponent), entt::forward_as_meta(registery), entity);
			}
			else
			{
				ASSERT_FAIL("The component " << WRAP(metaComponent.type().info().name(), "'") << " does not have an 'Assign' method!");
			}
		}
	} // namespace nabi::Reflection::Creation

	// --- Entity Creator ---

	EntityCreator::EntityCreator(entt::registry& registery) NABI_NOEXCEPT
		: m_Registry(registery)
	{
	}

	void EntityCreator::AssignEntityStore(EntityTemplateStore const&& entityTemplateStore) NABI_NOEXCEPT
	{
		m_EntityTemplateStore = entityTemplateStore;
	}

	void EntityCreator::AssignEntityGroupStore(EntityGroupStore const&& entityGroupStore) NABI_NOEXCEPT
	{
		m_EntityGroupStore = entityGroupStore;
	}

	entt::entity EntityCreator::CreateEntity(EntityCreator::EntityCreationSettings const* const entityCreationSettingsPtr) NABI_NOEXCEPT
	{
		EntityCreationSettings entityCreationSettings = {};
		EntityTemplateData* entityTemplate = nullptr;

		if (entityCreationSettingsPtr != nullptr)
		{
			entityCreationSettings = *entityCreationSettingsPtr;

			if (entityCreationSettingsPtr->m_EntityTemplateName != "")
			{
				std::string const& entityTemplateName = entityCreationSettings.m_EntityTemplateName;
				ASSERT_FATAL(m_EntityTemplateStore.find(entityTemplateName) != m_EntityTemplateStore.end(),
					"Trying to create an entity, " << WRAP(entityTemplateName, "'") << ", which doesn't exist in the entity store!");

				entityTemplate = &m_EntityTemplateStore.at(entityTemplateName.data());
			}
		}

		std::string& entityName = entityCreationSettings.m_EntityName;
		entt::entity const entity = m_Registry.create();

		if (entityCreationSettings.m_AppendUUID)
		{
			entityName += "_" + Utils::ECSUtils::GetEntityUUIDAsString(entity);
		}

		if (entityCreationSettings.m_EntityOverriddenProperties != nullptr)
		{
			EntityPropertyList const& overridenProperties = *entityCreationSettings.m_EntityOverriddenProperties;
			ASSERT_FATAL(entityTemplate, "Trying to override an entity's component's properties but the entityTemplate is null!");
			ResolveEntityTemplateComponents(*entityTemplate, overridenProperties);
		}

		if (entityTemplate != nullptr)
		{
			Creation::ResolveEntityComponents(entityTemplate->m_Components, m_Registry, entity);
		}
		Creation::AddEntityInfoComponentToEntity(m_Registry, entity, entityCreationSettings.m_EntityGroup, entityName);
		Creation::AddSpatialHierarchyComponentToEntity(m_Registry, entity);

		return entity;
	}

	bool EntityCreator::DestroyEntity(entt::entity& entity) NABI_NOEXCEPT
	{
		bool const entityExists = m_Registry.valid(entity);

		if (entityExists)
		{
			m_Registry.destroy(entity);
			entity = entt::null;
		}
		else
		{
			ASSERT_FAIL("Trying to destroy an entity which isn't in the registry!");
		}

		return entityExists;
	}

	entt::entity EntityCreator::CloneEntity(entt::entity const entityToClone) NABI_NOEXCEPT
	{
		// Again, note the warning in the header file! May cause unexpected behaviour when dealing with pointers and the like.

		entt::entity const clone = m_Registry.create();

		for (auto&& currentStorage : m_Registry.storage()) 
		{ 
			if (auto& storage = currentStorage.second; storage.contains(entityToClone)) 
			{
				storage.emplace(clone, storage.get(entityToClone));
			}
		}

		ecs::EntityInfoComponent entityInfoComponent = m_Registry.get<ecs::EntityInfoComponent>(clone);
		std::string const clonedName = std::string(entityInfoComponent.m_EntityName.data()) + std::string("_clone");
		entityInfoComponent.m_EntityName = entt::hashed_string(clonedName.c_str());

		return clone;
	}

	EntityGroup EntityCreator::CreateEntityGroup(std::string const& entityGroupName) NABI_NOEXCEPT
	{
		EntityGroup returnEntityGroup = {};

		auto entityGroup = m_EntityGroupStore.find(entityGroupName);
		if (entityGroup != m_EntityGroupStore.end())
		{
			std::vector<EntityData> const& entitiesInGroup = entityGroup->second;
			returnEntityGroup.GetGroup().reserve(entitiesInGroup.size());

			EntityCreationSettings entityCreationSettings = {};
			// Could be useful to extract the entity's name one day?
			entityCreationSettings.m_EntityGroup = entt::hashed_string(entityGroupName.c_str());

			for (EntityData const& entityToCreate : entitiesInGroup)
			{
				entt::entity const entity = CreateEntity(&entityCreationSettings);
				std::vector<ComponentData> const& entityComponents = entityToCreate.m_Components;
				Creation::ResolveEntityComponents(entityToCreate.m_Components, m_Registry, entity);

				returnEntityGroup.AddEntity(entity);
			}
		}
		else
		{
			ASSERT_FAIL("Trying to create an entity group which doesn't exist!");
		}

		return returnEntityGroup;
	}

	void EntityCreator::CreateEntityGroup(EntityGroup const& /*entityGroup*/) NABI_NOEXCEPT
	{
		FUNCTION_NOT_IMPLEMENTED
	}

	bool EntityCreator::DestroyEntityGroup(std::string const& entityGroupName) NABI_NOEXCEPT
	{
		entt::hashed_string const hashedGroupName = entt::hashed_string(entityGroupName.c_str());
		bool validGroup = false;

		m_Registry.view<ecs::EntityInfoComponent>()
			.each([&](entt::entity const entity, auto const& entityInfoComponent)
				{
					if (entityInfoComponent.m_EntityGroup == hashedGroupName)
					{
						DestroyEntity(entity);
						validGroup = true;
					}
				});

		if (!validGroup)
		{
			ASSERT_FAIL("Trying to destroy an entity group which has no entities or doesn't exist!");
		}
		return validGroup;
	}

	void EntityCreator::DestroyEntityGroup(EntityGroup& /*entityGroup*/) NABI_NOEXCEPT
	{
		FUNCTION_NOT_IMPLEMENTED
	}

	size_t EntityCreator::GetEntityStoreSize() const NABI_NOEXCEPT
	{
		return m_EntityTemplateStore.size();
	}

	size_t EntityCreator::GetEntityGroupStoreSize() const NABI_NOEXCEPT
	{
		return m_EntityGroupStore.size();
	}

	void EntityCreator::ResolveEntityTemplateComponents(EntityTemplateData& entityTemplateData, EntityPropertyList const& entityOverriddenProperties) NABI_NOEXCEPT
	{
		/*
		  bloody hell
		  it is all compairing / assigning ints under the hood
		  but still this is kinda rough
		  O(10000000)
		*/

		// Foreach property in the property list
		for (EntityPropertyList::PropertyComponentPair const& propertyComponentPair : entityOverriddenProperties.GetOverridenProperties())
		{
			Creation::OverrideEntityComponents(
				entityTemplateData.m_Components,                  // Components on the entity
				propertyComponentPair.m_ComponentHash,            // Component which the property override is referencing
				propertyComponentPair.m_PropertyOverride.m_Id,    // Property Id which the property override is refencing
				propertyComponentPair.m_PropertyOverride.m_Value, // The new value of the property
				&propertyComponentPair.m_PropertyOverride         // If the property doesn't exist on the component, what to add
			);
		}
	}
} // namespace nabi::Reflection
