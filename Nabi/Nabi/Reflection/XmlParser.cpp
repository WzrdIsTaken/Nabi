#include "XmlParser.h"

#include "../Libraries/pugixml/pugixml.hpp"

#include "../ECS/CoreComponents/EntityInfoComponent.h"
#include "../Utils/BuildUtils.h"
#include "../Utils/DebugUtils.h"
#include "../Utils/TestUtils.h"
#include "MetaObjectLookup.h"
#include "ReflectionGlobals.h"
#include "ReflectionHelpers.h"
#include "StringConverter.h"

namespace nabi::Reflection
{
	void XmlParser::ParseXml(std::string_view const routeDocPath, entt::registry& registery, MetaObjectLookup* const systemsLookup)
	{
		// Load the route document
		pugi::xml_document const routeDoc = LoadDocument(routeDocPath);
		LOG(LOG_PREP, LOG_INFO, "Loaded a route document " << WRAP(routeDocPath, "'") << " successfully!" << ENDLINE);

		for (pugi::xml_node const pathNode : routeDoc.child(c_RouteDocAttribute.c_str()))
		{
			// Go through each of the path nodes and load the doc associated with them
			std::string_view const path = pathNode.attribute(c_ValueAttribute.c_str()).value();
			pugi::xml_document const dataDoc = LoadDocument(path);
			LOG(LOG_PREP, LOG_INFO, "Loaded a data document " << WRAP(path, "'") << " successfully!" << ENDLINE);

			// Take a peak at the type of that document
			std::string_view const docType = dataDoc.first_child().name();

			// Depending on the document's type, call the relevant parse function
			if (docType == c_RouteDocAttribute)
			{
				ParseXml(path, registery, systemsLookup);
			}
			else if (docType == c_SingletonGroupAttribute)
			{
				ParseSingletons(dataDoc);
			}
			else if (docType == c_SystemGroupAttribute)
			{
				ParseSystems(dataDoc, registery, systemsLookup);
			}
			else if (docType == c_EntityGroupAttribute)
			{
				ParseEntities(dataDoc, registery);
			}
			else
			{
				ASSERT_FAIL("The document type isn't valid!");
			}
		}
	}

	pugi::xml_document XmlParser::LoadDocument(std::string_view const docPath)
	{
		pugi::xml_document doc;
		pugi::xml_parse_result const result = doc.load_file(docPath.data());

		ASSERT_FATAL(result, "The document " << WRAP(docPath, "'") << " could not be found!");
		return doc;
	}

	std::unordered_map<std::string, EntityTemplateData>& XmlParser::GetEntityTemplateStore()
	{
		return m_EntityTemplates;
	}

	void XmlParser::ParseSingletons(pugi::xml_document const& /*doc*/)
	{
		FUNCTION_NOT_IMPLEMENTED

		// I dont know how singletons will work
		// And I don't need them right now (or ever??), so:
		// - If I have a sudden brainwave I will make them
		// - Else I will think about them when I actually need to 
	}

	void XmlParser::ParseSystems(pugi::xml_document const& doc, entt::registry& registery, MetaObjectLookup* const systemsLookup)
	{
		// Get the system group's id
		std::string_view const systemGroupId = doc.first_child().attribute(c_IdAttribute.c_str()).value();
		entt::hashed_string const systemGroupIdHash = entt::hashed_string(systemGroupId.data());
		LOG(LOG_PREP, LOG_INFO, "Found a system group with id " << WRAP(systemGroupId, "'") << ENDLINE);

		// Iterate through all of the systems in the group
		for (pugi::xml_node const systemNode : doc.child(c_SystemGroupAttribute.c_str()))
		{
			// Check if the system is debug only
			if (!CheckIfNodeHasDebugPropertyAndConfigurationIsDebug(systemNode))
			{
				// Get the system's id
				std::string_view const systemId = systemNode.attribute(c_IdAttribute.c_str()).value();
				entt::hashed_string const systemIdHash = entt::hashed_string(systemId.data());
				LOG(LOG_PREP, LOG_INFO, SPACE(INDENT_1) << "Created a system with id " << WRAP(systemId, "'") << ENDLINE);

				// Construct the system
				entt::meta_any metaSystem = ReflectionHelpers::ConstructMetaObject(systemIdHash, entt::forward_as_meta(registery), systemIdHash, systemGroupIdHash);

				// Construct the data for the ststem
				SystemData const systemData = CreateECSTypeData(systemNode);

				// Resolve any properties on the system
				ResolveProperties(systemData, metaSystem);

				// Add the system to the system's lookup map
				if (systemsLookup != nullptr)
				{
					systemsLookup->AddObject(systemId.data(), metaSystem);
				}
			}
		}
	}

	void XmlParser::ParseEntities(pugi::xml_document const& doc, entt::registry& registery)
	{
		// Get the entity group's id
		std::string_view const entityGroupId = doc.first_child().attribute(c_IdAttribute.c_str()).value();
		entt::hashed_string const entityGroupIdHash = entt::hashed_string(entityGroupId.data());
		LOG(LOG_PREP, LOG_INFO, "Found a entity group with id " << WRAP(entityGroupId, "'") << ENDLINE);

		// Loop through all of the entities in the group
		for (pugi::xml_node entityNode : doc.child(c_EntityGroupAttribute.c_str()))
		{
			// Check if the entity is debug only
			if (!CheckIfNodeHasDebugPropertyAndConfigurationIsDebug(entityNode))
			{
				// Check if the entity is a template or not
				bool const isEntityTemplate = entityNode.name() == c_EntityTemplateAttribute;
				if (isEntityTemplate)
				{
					ResolveEntityTemplate(entityNode);
				}
				else
				{
					ResolveEntity(entityNode, entityGroupIdHash, registery);
				}
			}
		}
	}

	void XmlParser::ResolveEntityTemplate(pugi::xml_node const& entityTemplateNode)
	{
		// Get the entity template's name
		std::string const entityTemplateName = entityTemplateNode.attribute(c_IdAttribute.c_str()).value();
		LOG(LOG_PREP, LOG_INFO, SPACE(INDENT_1) << "Found an entity template with name " << WRAP(entityTemplateName, "'") << ENDLINE);

		// Create an entry in m_EntityTemplates and get a reference to the templates components
		EntityTemplateData entityTemplateData;
		entityTemplateData.m_Id = entt::hashed_string(entityTemplateName.data());
		entityTemplateData.m_Components = {};

		m_EntityTemplates.insert({ entityTemplateName, entityTemplateData });
		std::vector<ComponentData>& entityTemplateComponents = m_EntityTemplates.at(entityTemplateName).m_Components;

		// Check if the entity template is a template of something else
		pugi::xml_attribute const entityTemplate = entityTemplateNode.attribute(c_TemplateAttribute.c_str());
		if (entityTemplate)
		{
			// Find the entity template's template
			pugi::char_t const* const entityTemplateParentName = entityTemplate.value();
			LOG(LOG_PREP, LOG_INFO, SPACE(INDENT_2) << "It inherits from " << WRAP(entityTemplateParentName, "'") << ENDLINE);

			ASSERT_FATAL(m_EntityTemplates.find(entityTemplateParentName) != m_EntityTemplates.end(),
				"A template with the id " << WRAP(entityTemplateParentName, "'") << " does not exist in m_EntityTemplates!");

			// Assign all the parents components to this entity template
			std::vector<ComponentData> const& parentTemplateComponents = m_EntityTemplates.at(entityTemplateParentName).m_Components;
			for (ComponentData const& component : parentTemplateComponents)
			{
				entityTemplateComponents.push_back(component);
			}
		}
		
		// Resolve the components and properties on *this* entity template
		ResolveComponentOrPropertyNode(entityTemplateNode, entityTemplateComponents);
	}

	void XmlParser::ResolveEntity(pugi::xml_node const& entityNode, entt::hashed_string const entityGroupIdHash, entt::registry& registery)
	{
		// Create an entity
		entt::entity const entity = registery.create();
		std::string_view const entityId = entityNode.attribute(c_IdAttribute.c_str()).value();
		LOG(LOG_PREP, LOG_INFO, SPACE(INDENT_1) << "Created an entity with name " << WRAP(entityId, "'") << ENDLINE);

		// Create a collection of all the entities components
		std::vector<ComponentData> entityComponents{};

		// Check if the entity has a template
		pugi::xml_attribute const entityTemplate = entityNode.attribute(c_TemplateAttribute.c_str());
		if (entityTemplate)
		{
			// If the entity has a template, add all of the template entity's components to this entity
			pugi::char_t const* const entityTemplateId = entityTemplate.value();
			ASSERT_FATAL(m_EntityTemplates.find(entityTemplateId) != m_EntityTemplates.end(),
				"The Entity Template " << entityTemplateId << "does not exist!");

			std::vector<ComponentData>& entityTemplateComponents = m_EntityTemplates.at(entityTemplateId).m_Components;
			for (ComponentData const& entityTemplateComponent : entityTemplateComponents)
			{
				entityComponents.push_back(entityTemplateComponent);
			}
		}

		// Resolve the components and properties on *this* entity
		ResolveComponentOrPropertyNode(entityNode, entityComponents);

		// Spin through the entity components, resolve them and add all them all to the registery
		for (ComponentData const& component : entityComponents)
		{
			// Get the component's ID and resolve it
			std::string_view const componentId = component.m_Id.data();
			entt::meta_any metaComponent = ResolveComponent(component, componentId);
			LOG(LOG_PREP, LOG_INFO, SPACE(INDENT_2) << "Found a component on the entity with id " << WRAP(componentId, "'") << ENDLINE);

			// Assign the component to the registery
			AssignComponentToRegistery(metaComponent, registery, entity);
		}

		// Add the EntityInfoComponent to the entity
		AddEntityInfoComponentToEntity(registery, entity, entityGroupIdHash, entityId);
	}

	void XmlParser::ResolveEntityComponents(std::vector<ComponentData>& componentData, pugi::xml_node const& propertyNode)
	{
		ASSERT(propertyNode.attribute(c_ComponentAttribute.c_str()), "The property node doesn't have a component ref!");

		// Get infomation about the property
		entt::hashed_string const propertyComponentRef = entt::hashed_string(propertyNode.attribute(c_ComponentAttribute.c_str()).value());
		entt::hashed_string const propertyId = entt::hashed_string(propertyNode.attribute(c_IdAttribute.c_str()).value());
		entt::hashed_string const propertyValue = entt::hashed_string(propertyNode.attribute(c_ValueAttribute.c_str()).value());

		// Loop through all of the component's properties
		for (ComponentData& componentNode : componentData)
		{
			if (componentNode.m_Id == propertyComponentRef)
			{
				for (PropertyData& propertyNode : componentNode.m_Properties)
				{
					// If we find the property on one of the component data, override it
					if (propertyNode.m_Id == propertyId)
					{
						propertyNode.m_Value = propertyValue;
						goto ResolveEntityComponentsExit;
					}
				}

				// If we didn't find it, but the property links to this component, it just means that the property has not been set in xml
				// and will be default initialized. To override that default initialization, add the property data to the component data
				PropertyData propertyData;
				propertyData.m_Id = propertyId;
				propertyData.m_Value = propertyValue;

				componentNode.m_Properties.push_back(propertyData);
				goto ResolveEntityComponentsExit;
			}
		}

		ASSERT_FAIL("The property is referencing a component which doesn't exist!");

	ResolveEntityComponentsExit:
		LOG(LOG_PREP, LOG_INFO, SPACE(INDENT_2) << "Found a property node with id " << WRAP(propertyId.data(), "'") <<
			" overriding a value on " << WRAP(propertyComponentRef.data(), "'") << " with value " << WRAP(propertyValue.data(), "'") << ENDLINE);
	}

	void XmlParser::ResolveComponentOrPropertyNode(pugi::xml_node const& entityNode, std::vector<ComponentData>& entityComponents)
	{
		// Iterate through the entity node's children
		for (pugi::xml_node const componentOrPropertyNode : entityNode.children())
		{
			// Check if the component or property is debug only
			if (!CheckIfNodeHasDebugPropertyAndConfigurationIsDebug(componentOrPropertyNode))
			{
				// Check if the node is a property node
				bool const isPropertyNode = componentOrPropertyNode.name() == c_PropertyAttribute;
				if (isPropertyNode)
				{
					// If the node is a property node, then we want to override an assigned components property with its value
					ResolveEntityComponents(entityComponents, componentOrPropertyNode);
					continue;
				}

				// Otherwise, the node must be a component node. Create a component for this data
				ComponentData const component = CreateECSTypeData(componentOrPropertyNode);

				// Entt already doesn't allow this, but this assert is more explicit
				ASSERT(std::find(entityComponents.begin(), entityComponents.end(), component) == entityComponents.end(),
					"The entity already has a component of type " << WRAP(component.m_Id.data(), "'") << "!" << ENDLINE);

				// Push back the created component into the entity templates component list
				entityComponents.push_back(component);
				LOG(LOG_PREP, LOG_INFO, SPACE(INDENT_2) << "Found a component on the entity template with id " << WRAP(component.m_Id.data(), "'") << ENDLINE);
			}
		}
	}

	entt::meta_any XmlParser::ResolveComponent(ComponentData const& componentData, std::string_view const componentId)
	{
		// Resolve the component
		entt::meta_any metaComponent = ReflectionHelpers::ConstructMetaObject(componentId);

		// Iterate through all of the component's nodes children - these will be the component's properties 
		ResolveProperties(componentData, metaComponent);

		// Return the created component
		return metaComponent;
	}

	void XmlParser::ResolveProperties(MetaECSTypeData const& data, entt::meta_any& metaObject)
	{
		// Spin through all the system/components properties
		for (PropertyData const& propertyNode : data.m_Properties)
		{
			// Get the property's infomation
			std::string_view const propertyId = propertyNode.m_Id.data();
			std::string_view const propertyValue = propertyNode.m_Value.data();
			LOG(LOG_PREP, LOG_INFO, SPACE(INDENT_2) << "Found a property with id " << WRAP(propertyId, "'") << " and value " << WRAP(propertyValue, "'") << ENDLINE);

			// Resolve the property
			ResolveProperty(metaObject, propertyId, propertyValue);
		}
	}

	void XmlParser::ResolveProperty(entt::meta_any& metaObject, std::string_view const propertyName, std::string_view const propertyValue)
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

	void XmlParser::AddEntityInfoComponentToEntity(entt::registry& registey, entt::entity const entity,
		entt::hashed_string const& entityGroupHash, std::string_view const entityName)
	{
		auto entityInfoComponentSettings = ecs::entityInfoComponentDefaultSettings;
		entityInfoComponentSettings.m_EntityGroup = entityGroupHash;
		entityInfoComponentSettings.m_EntityName = entt::hashed_string(entityName.data());

		registey.emplace<ecs::EntityInfoComponent>(entity, entityInfoComponentSettings);
	}

	void XmlParser::AssignComponentToRegistery(entt::meta_any& metaComponent, entt::registry& registery, entt::entity const entity)
	{
		// Find the assign function and call it on the specified entity
		entt::hashed_string const assignFunctionHash = entt::hashed_string(ReflectionGlobals::c_AssignFunctionName);
		if (entt::meta_func const assign = metaComponent.type().func(assignFunctionHash))
		{
			assign.invoke(metaComponent, entt::forward_as_meta(metaComponent), entt::forward_as_meta(registery), entity);
		}
		else
		{
			ASSERT_FAIL("The component " << WRAP(metaComponent.type().info().name(), "'") << " does not have an 'Assign' method!");
		}
	}

	bool XmlParser::CheckIfNodeHasDebugPropertyAndConfigurationIsDebug(pugi::xml_node const& node)
	{
		MAKE_LOGIC_TESTABLE // (test in ReflectionDebugAttributeTests.cpp)
		{
			pugi::xml_attribute const debugAttribute = node.attribute(c_DebugAttribute.c_str());

			// Can early out if the node doesn't have the property 
			if (debugAttribute)
			{
				// Else check the properties value
				bool const debugPropertyValue = StringConverter::FromString<bool>(debugAttribute.value());
				return debugPropertyValue;
			}
		}

		return false;
	}

	MetaECSTypeData XmlParser::CreateECSTypeData(pugi::xml_node const& node)
	{
		std::vector<PropertyData> componentProperties;
		for (pugi::xml_node const propertyNode : node.children())
		{
			// Check if the property is debug only
			if (!CheckIfNodeHasDebugPropertyAndConfigurationIsDebug(propertyNode))
			{
				PropertyData const propertyData = CreatePropertyData(propertyNode);
				componentProperties.push_back(propertyData);
			}
		}

		std::string_view const nodeId = node.attribute(c_IdAttribute.c_str()).value();

		MetaECSTypeData ecsTypeData; // Note - MetaECSTypeData can refer to a System or a Component
		ecsTypeData.m_Id = entt::hashed_string(nodeId.data());
		ecsTypeData.m_Properties = componentProperties;

		return ecsTypeData;
	}

	PropertyData XmlParser::CreatePropertyData(pugi::xml_node const& node)
	{
		std::string_view const nodeId = node.attribute(c_IdAttribute.c_str()).value();
		std::string_view const nodeValue = node.attribute(c_ValueAttribute.c_str()).value();

		PropertyData propertyData;
		propertyData.m_Id = entt::hashed_string(nodeId.data());
		propertyData.m_Value = entt::hashed_string(nodeValue.data());

		return propertyData;
	}
} // namespace nabi::Reflection
