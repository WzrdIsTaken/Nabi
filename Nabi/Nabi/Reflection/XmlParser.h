#pragma once

#include <string>

#include "../Libraries/entt/entt.h" // Unfortunately not possible to forward declare entt in this case, due to pass by value of entt::entity (because under the hood its just a uint32)

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
	class XmlParser
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
		/// Resolves properties on a system or entity. Ie, this process takes the string property id, value and assigns the specified value to the specified meta property.
		/// </summary>
		/// <param name="node">- The system or entity node on which the propertie reside</param>
		/// <param name="metaObject">- The object to which the properties belong to</param>
		void ResolveProperties(pugi::xml_node const& node, entt::meta_any& metaObject);
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

		// File Specifiers 
		std::string const c_RouteDocAttribute = "route";
		std::string const c_SingletonGroupAttribute = "singleton_group";
		std::string const c_SystemGroupAttribute = "system_group";
		std::string const c_EntityGroupAttribute = "entity_group";

		// Types
		std::string const c_SingletonAttribute = "singleton";
		std::string const c_SystemAttribute = "system";
		std::string const c_EntityAttribute = "entity";

		std::string const c_ComponentAttribute = "component";
		std::string const c_PropertyAttribute = "property";

		// Properties
		std::string const c_IdAttribute = "id";
		std::string const c_ValueAttribute = "value";
	};
} // namespace nabi::Reflection
