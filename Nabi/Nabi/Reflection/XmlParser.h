#pragma once

#include <string>

#include "../Libraries/entt/entt.h" // Unfortunately not possible to forward declare entt in this case, due to pass by value of entt::entity (because under the hood its just a uint32)
#include "MetaECSTypes.h"

// Forward declarations
namespace pugi
{
	class xml_document;
	class xml_node;
} // namespace pugi
namespace nabi::Reflection
{
	class MetaObjectLookup;
} // namespace nabi::Reflection

namespace nabi::Reflection
{
	/// <summary>
	/// Does exactly what it says on the tin. Can currently be used to parse route, component and system xml data files.
	/// </summary>
	class XmlParser final
	{
	public:
		/// <summary>
		/// Parses an xml document, determines the type of the data inside and passes it onto the relevant function to handle that data.
		/// Will be called recursivly for nested route docs.
		/// </summary>
		/// <param name="routeDoc">- The first route document, likely containing file paths to other data documents</param>
		/// <param name="registery">- A reference to the registery, forwarded on to the ParseSystems and ParseComponents functions</param>
		/// <param name="systemsLookup">- An optional MetaObjectLookup, which systems will be added to. Pass in nullptr if it is not required</param>
		void ParseXml(std::string_view const routeDoc, entt::registry& registery, MetaObjectLookup* const systemsLookup);
		/// <summary>
		/// Loads the document at the specified path.
		/// </summary>
		/// <param name="docPath">- The document to load</param>
		/// <returns>The loaded xml document</returns>
		pugi::xml_document LoadDocument(std::string_view const docPath);

		/// <summary>
		/// Gets the all of the entity templates that have been read.
		/// Should ideally be called once at the end of xml parsing and used to set up EntityCreator
		/// </summary>
		/// <returns>The entity template store</returns>
		std::unordered_map<std::string, EntityTemplateData>& GetEntityTemplateStore();

		/// <summary>
		/// Parses a document containing singleton data. Note: Currently not implemented.
		/// </summary>
		/// <param name="doc">- The singleton data document</param>
		void ParseSingletons(pugi::xml_document const& doc);
		/// <summary>
		/// Parses a document containing system data. System documents are designated by c_SystemGroupAttribute's value.
		/// </summary>
		/// <param name="doc">- The system data document</param>
		/// <param name="registery">- A reference to the registery, required to initialise systems</param>
		/// <param name="systemsLookup">- An optional MetaObjectLookup, which systems will be added to. Pass in nullptr if it is not required</param>
		void ParseSystems(pugi::xml_document const& doc, entt::registry& registery, MetaObjectLookup* const systemsLookup);
		/// <summary>
		/// Parses a document containing entity data. Component documents are designated by c_EntityGroupAttribute's value.
		/// </summary>
		/// <param name="doc">- The entity data document</param>
		/// <param name="registery">- A reference to the registery, required to assign the component's to the registery</param>
		void ParseEntities(pugi::xml_document const& doc, entt::registry& registery);

	private:
		/// <summary>
		/// Resolves an entity template and adds it to m_EntityTemplates
		/// </summary>
		/// <param name="entityTemplateNode">- The xml node of the entity template</param>
		void ResolveEntityTemplate(pugi::xml_node const& entityTemplateNode);
		/// <summary>
		/// Resolves an entity
		/// </summary>
		/// <param name="node">- The xml node of the entity</param>
		/// <param name="entityGroupIdHash">- The group the entity belongs to</param>
		/// <param name="registery">- Reference to the registery</param>
		void ResolveEntity(pugi::xml_node const& node, entt::hashed_string const entityGroupIdHash, entt::registry& registery);

		/// <summary>
		/// Checks to see if the property exists on any of the passed in components, and if it does overrides its value with the property's value
		/// </summary>
		/// <param name="componentData">- A vector of all the components on an entity or entity template</param>
		/// <param name="propertyNode">- The xml node of a property</param>
		void ResolveEntityComponents(std::vector<ComponentData>& componentData, pugi::xml_node const& propertyNode);
		/// <summary>
		/// Checks to see if a child nodes on an entity or entity template is a property or component and resolves it accordingly. 
		/// ResolveEntityComponents is called for properties, for components they are just added to the entity or entity template
		/// </summary>
		/// <param name="entityNode">- The xml node of the entity</param>
		/// <param name="entityComponents">- A reference to the entities or entity template's component data</param>
		void ResolveComponentOrPropertyNode(pugi::xml_node const& entityNode, std::vector<ComponentData>& entityComponents);

		/// <summary>
		/// Creates a meta component from component data
		/// </summary>
		/// <param name="componentData">- The data for the component</param>
		/// <param name="componentId">- The component's id. For looking up its type descriptor</param>
		/// <returns>The meta component</returns>
		entt::meta_any ResolveComponent(ComponentData const& componentData, std::string_view const componentId);
		/// <summary>
		/// Resolves properties on a system or entity. Ie, this process takes the string property id, value and assigns the specified value to the specified meta property.
		/// </summary>
		/// <param name="data">- The system or entity node on which the propertie reside</param>
		/// <param name="metaObject">- The object to which the properties belong to</param>
		void ResolveProperties(MetaECSTypeData const& data, entt::meta_any& metaObject);
		/// <summary>
		/// Assigns the specified property to the passed in metaObject.
		/// </summary>
		/// <param name="metaComponent">- The meta object to assign the property to</param>
		/// <param name="propertyName">- The name of the property</param>
		/// <param name="propertyValue">- The value of the property</param>
		void ResolveProperty(entt::meta_any& metaObject, std::string_view const propertyName, std::string_view const propertyValue);

		/// <summary>
		/// Adds the EntityInfoComponent to an entity. This component contains some basic data about the entity.
		/// </summary>
		/// <param name="registey">- A reference to the registery, required for assigning the component to the entity</param>
		/// <param name="entity">- The entity to assign the component to</param>
		/// <param name="entityGroupHash">- The hash of the entity group (hash as its already in this form from ParseEntities</param>
		/// <param name="entityName">- The entity's name</param>
		void AddEntityInfoComponentToEntity(entt::registry& registey, entt::entity const entity,
			entt::hashed_string const& entityGroupHash, std::string_view const entityName);

		/// <summary>
		/// Calls the Assign function on a component's Reflector, which emplaces the component to the entity
		/// </summary>
		/// <param name="metaComponent">- The component to assign to the entity</param>
		/// <param name="entity">- The entity</param>
		/// <param name="registery">- A reference to the registery</param>
		void AssignComponentToRegistery(entt::meta_any& metaComponent, entt::registry& registery, entt::entity const entity);

		/// <summary>
		/// Checks if a node has the c_DebugAttribute attribute and its value is "true"
		/// Debug [systems/entities/components/properties] are only created in debug mode
		/// Then, checks if we are in debug mode. If both these things are true, then the node is debug only
		/// </summary>
		/// <param name="node">- The node to check</param>
		/// <returns>If the node has a debug attribute set to true</returns>
		bool CheckIfNodeHasDebugPropertyAndConfigurationIsDebug(pugi::xml_node const& node);

		/// <summary>
		/// Creates a MetaECSTypeData from an xml node
		/// Note - MetaECSTypeData can be a system or component. See MetaECSTypes.h for more infomation
		/// </summary>
		/// <param name="node">- The xml node of the system or component</param>
		/// <returns>The created MetaECSTypeData</returns>
		MetaECSTypeData CreateECSTypeData(pugi::xml_node const& node);
		/// <summary>
		/// Creates a PropertyData from an xml node
		/// </summary>
		/// <param name="node">- The xml node of the property</param>
		/// <returns>The created PropertyData</returns>
		PropertyData CreatePropertyData(pugi::xml_node const& node);

		// Entity Template Stuff
		std::unordered_map<std::string, EntityTemplateData> m_EntityTemplates; // (template name - template)

		// File Specifiers 
		std::string const c_RouteDocAttribute = "route";
		std::string const c_SingletonGroupAttribute = "singleton_group";
		std::string const c_SystemGroupAttribute = "system_group";
		std::string const c_EntityGroupAttribute = "entity_group";

		// Types
		std::string const c_SingletonAttribute = "singleton";
		std::string const c_SystemAttribute = "system";
		std::string const c_EntityAttribute = "entity";
		std::string const c_EntityTemplateAttribute = "entity_template";

		std::string const c_ComponentAttribute = "component";
		std::string const c_PropertyAttribute = "property";

		// Properties
		std::string const c_TemplateAttribute = "template";

		std::string const c_IdAttribute = "id";
		std::string const c_ValueAttribute = "value";

		// Debug
		std::string const c_DebugAttribute = "debug";
	};
} // namespace nabi::Reflection
