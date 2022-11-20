#include "XmlParser.h"

#include "../Libraries/pugixml/pugixml.hpp"

#include "../ECS/CoreComponents/EntityInfoComponent.h"
#include "../Utils/DebugUtils.h"
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
				ASSERT(false, "The document type isn't valid!");
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

	void XmlParser::ParseSingletons(pugi::xml_document const& /*doc*/)
	{
		// I dont know how singletons will work
		// And I don't need them right now (or ever??), so:
		// - If I have a sudden brainwave I will make them
		// - Else I will think about them when I actually need to 
	}

	void XmlParser::ParseSystems(pugi::xml_document const& doc, entt::registry& registery, MetaObjectLookup* const systemsLookup)
	{
		// Get the system group tab
		std::string_view const systemGroupId = doc.first_child().attribute(c_IdAttribute.c_str()).value();
		entt::hashed_string const systemGroupIdHash = entt::hashed_string(systemGroupId.data());
		LOG(LOG_PREP, LOG_INFO, "Found a system group with id " << WRAP(systemGroupId, "'") << ENDLINE);

		for (pugi::xml_node const systemNode : doc.child(c_SystemGroupAttribute.c_str()))
		{
			// Get the system's id
			std::string_view const systemId = systemNode.attribute(c_IdAttribute.c_str()).value();
			entt::hashed_string const systemIdHash = entt::hashed_string(systemId.data());
			LOG(LOG_PREP, LOG_INFO, SPACE(3) << "Created a system with id " << WRAP(systemId, "'") << ENDLINE);

			// Construct the system
			entt::meta_any metaSystem = ReflectionHelpers::ConstructMetaObject(systemIdHash, entt::forward_as_meta(registery), systemIdHash, systemGroupIdHash);

			// Resolve any properties on the system
			ResolveProperties(systemNode, metaSystem);

			// Add the system to the system's lookup map
			if (systemsLookup != nullptr)
			{
				systemsLookup->AddObject(systemId.data(), metaSystem);
			}
		}
	}

	void XmlParser::ParseEntities(pugi::xml_document const& doc, entt::registry& registery)
	{
		// Get the entity group tag
		std::string_view const entityGroupId = doc.first_child().attribute(c_IdAttribute.c_str()).value();
		entt::hashed_string const entityGroupIdHash = entt::hashed_string(entityGroupId.data());
		LOG(LOG_PREP, LOG_INFO, "Found a entity group with id " << WRAP(entityGroupId, "'") << ENDLINE);

		for (pugi::xml_node const entityNode : doc.child(c_EntityGroupAttribute.c_str()))
		{
			// Create an entity
			entt::entity const entity = registery.create();
			LOG(LOG_PREP, LOG_INFO, SPACE(3) << "Created an entity" << ENDLINE);

			// Add the EntityInfoComponent to the entity
			std::string_view const entityId = entityNode.attribute(c_IdAttribute.c_str()).value();
			AddEntityInfoComponentToEntity(registery, entity, entityGroupIdHash, entityId);

			// Iterate through the entity node's children - these will be the entity's components
			for (pugi::xml_node const componentNode : entityNode.children())
			{
				std::string_view const componentId = componentNode.attribute(c_IdAttribute.c_str()).value();
				LOG(LOG_PREP, LOG_INFO, SPACE(6) << "Found a component on the entity with id " << WRAP(componentId, "'") << ENDLINE);

				// Resolve the component
				entt::meta_any metaComponent = ReflectionHelpers::ConstructMetaObject(componentId);

				// Iterate through all of the component's nodes children - these will be the component's properties 
				ResolveProperties(componentNode, metaComponent);

				// Assign the component to the registery
				AssignComponentToRegistery(metaComponent, registery, entity);
			}
		}
	}

	void XmlParser::AddEntityInfoComponentToEntity(entt::registry& registey, entt::entity const entity,
		entt::hashed_string const& entityGroupHash, std::string_view const entityName)
	{
		auto entityInfoComponentSettings = EntityInfoComponent::entityInfoComponentDefaultSettings;
		entityInfoComponentSettings.m_EntityGroup = entityGroupHash;
		entityInfoComponentSettings.m_EntityName = entt::hashed_string(entityName.data());

		registey.emplace<EntityInfoComponent::EntityInfoComponent>(entity, entityInfoComponentSettings);
	}

	void XmlParser::ResolveProperties(pugi::xml_node const& node, entt::meta_any& metaObject)
	{
		// Spin through all the system/components properties
		for (pugi::xml_node const propertyNode : node.children())
		{
			// Get the property's infomation
			std::string_view const propertyId = propertyNode.attribute(c_IdAttribute.c_str()).value();
			std::string_view const propertyValue = propertyNode.attribute(c_ValueAttribute.c_str()).value();

			// Some debug only checks to make sure the logging is alright
#ifndef NDEBUG
			std::string_view const nodeType = node.name();
			if (nodeType == c_SystemAttribute)
			{
				LOG(LOG_PREP, LOG_INFO, SPACE(6) << "Found a property on the system with id " << WRAP(propertyId, "'") << " and value " << WRAP(propertyValue, "'") << ENDLINE);
			}
			else if (nodeType == c_ComponentAttribute)
			{
				LOG(LOG_PREP, LOG_INFO, SPACE(9) << "Found a property on the component with id " << WRAP(propertyId, "'") << " and value " << WRAP(propertyValue, "'") << ENDLINE);
			}
			// else if Singletons soontm??
#endif // ifndef NDEBUG

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
			ASSERT(false, "The component " << WRAP(metaComponent.type().info().name(), "'") << " does not have an 'Assign' method!");
		}
	}
} // namespace nabi::Reflection
