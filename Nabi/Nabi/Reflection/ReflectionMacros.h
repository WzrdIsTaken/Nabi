#pragma once

// Macros to aid in the reflection of components, systems and data types

// --- Reflector Class ---
// The reflector class contains a pseudo static constructor which calls the InitReflection method - which does exactly what you think.
// It also wraps the Assign method for components.
// As it contains a static instance, we also need to declare it somewhere in the cpp - hense the CREATE_REFLECTOR_INSTANCE macro

#define CREATE_REFLECTOR_BEGIN(typeName) \
	class CONCAT(typeName, Reflector) \
	{ \
		friend class ReflectorConstructor; \
		\
		class ReflectorConstructor \
		{ \
		public: \
			ReflectorConstructor() \
			{ \
				InitReflection(); \
			} 

#define CREATE_REFLECTOR_END() \
		}; \
		\
		static ReflectorConstructor reflectedConstructor; \
	};

#define CREATE_REFLECTOR_INSTANCE(typeName) CONCAT(typeName, Reflector::ReflectorConstructor) CONCAT(typeName, Reflector::reflectedConstructor);

// --- Helper ---
// A couple of helper macros to do operations which are done multiple times in multiple places.
// The REFLECT_PRIVATES macro just friends the Reflector class, so it can access (normally systems, as they are classes) properties for reflection.

#define CREATE_INIT_REFLECTION_FUNCTION() \
	private: \
	static void InitReflection() \
	{

#define REFLECT_PRIVATES(typeName) \
	friend class CONCAT(typeName, Reflector);

// --- Component Reflection ---
// The assign function for components emplaces the component to the passed in entity in the registry.
// As for the general flow of reflection, we begin, reflect the properties and end. Begin/end just create the reflector class. 
// The DEFAULT variation of the macros stringifys the type name.

// Assign function
#define CREATE_ASSIGN_FUNCTION(componentType) \
	static void Assign(entt::meta_any const& metaComponent, entt::registry& registry, entt::entity const entity) \
	{ \
		using T = componentType; \
		using U = CONCAT(componentType, Settings); \
		\
		T const component = metaComponent.cast<T>(); \
		U const settings = ConstructSettingsFromComponent(component); \
		\
		registry.emplace<T>(entity, settings); \
	}

	// Begin
#define REFLECT_COMPONENT_BEGIN_DEFAULT(component) \
	CREATE_REFLECTOR_BEGIN(component) \
	CREATE_ASSIGN_FUNCTION(component) \
	\
	CREATE_INIT_REFLECTION_FUNCTION() \
		entt::meta<component>().type(entt::hashed_string(#component)) \
			.ctor<>()

#define REFLECT_COMPONENT_BEGIN(component, reflectedName) \
	CREATE_REFLECTOR_BEGIN(component) \
	CREATE_ASSIGN_FUNCTION(component) \
	\
	CREATE_INIT_REFLECTION_FUNCTION() \
		entt::meta<component>().type(entt::hashed_string(reflectedName)) \
			.ctor<>()

// Properties
#define REFLECT_COMPONENT_PROPERTY_DEFAULT(member) \
			.data<&member>(entt::hashed_string(member))

#define REFLECT_COMPONENT_PROPERTY(member, reflectedName) \
			.data<&member>(entt::hashed_string(reflectedName))

// End
#define REFLECT_COMPONENT_END() \
			.func<&Assign>(nabi::Reflection::ReflectionGlobals::c_AssignFunctionName); \
	} \
	CREATE_REFLECTOR_END()

// --- Data Type Reflection ---
// Data type (/just normal structs and classes) reflection works a bit differently to systems and components. 
// Their type just needs to be registered with the reflection system. However - note that all custom types need to define a FromString function.

#define REFLECT_DATA_TYPE_DEFAULT(dataType) \
	CREATE_REFLECTOR_BEGIN(dataType) \
	CREATE_INIT_REFLECTION_FUNCTION() \
		entt::meta<dataType>().type(entt::hashed_string(#dataType)) \
			.func<&dataType::FromString>(nabi::Reflection::ReflectionGlobals::c_FromStringFunctionName); \
	} \
	CREATE_REFLECTOR_END()

#define REFLECT_DATA_TYPE(dataType, reflectedName) \
	CREATE_REFLECTOR_BEGIN(dataType) \
	CREATE_INIT_REFLECTION_FUNCTION() \
		entt::meta<dataType>().type(entt::hashed_string(reflectedName)) \
			.func<&dataType::FromString>(nabi::Reflection::ReflectionGlobals::c_FromStringFunctionName); \
	} \
	CREATE_REFLECTOR_END()

// --- System Reflection ---
// System reflection works in exactly the same way as component reflection for a user.
// However under the hood the Assign method is not added to the Reflector class because it is not needed.
// Also, note - System constructors must take in a reference to the registery, a const system id and a const system group id.

// Begin
#define REFLECT_SYSTEM_BEGIN_DEFAULT(system) \
	CREATE_REFLECTOR_BEGIN(system) \
	CREATE_INIT_REFLECTION_FUNCTION() \
		entt::meta<system>().type(entt::hashed_string(#system)) \
			.ctor<entt::registry&, entt::hashed_string const, entt::hashed_string const>()

#define REFLECT_SYSTEM_BEGIN(system, reflectedName) \
	CREATE_REFLECTOR_BEGIN(system) \
	CREATE_INIT_REFLECTION_FUNCTION() \
		entt::meta<system>().type(entt::hashed_string(reflectedName)) \
			.ctor<entt::registry&, entt::hashed_string const, entt::hashed_string const>()

// Functions
#define REFLECT_SYSTEM_METHOD_DEFAULT(method) \
			.func<&method>(entt::hashed_string(#method))

#define REFLECT_SYSTEM_METHOD(method, reflectedName) \
			.func<&method>(entt::hashed_string(reflectedName))

// Properties
#define RELFECT_SYSTEM_PROPERTY_DEFAULT(member) \
			.data<&member>(entt::hashed_string(#member))

#define RELFECT_SYSTEM_PROPERTY(member, reflectedName) \
			.data<&member>(entt::hashed_string(reflectedName))

// End
#define RELFECT_SYSTEM_END() \
	; } \
	CREATE_REFLECTOR_END()

// --- Base Type Reflection ---
// A simple marcro to reflect base types. The ToString method for base types will always live on StringConverter.

#define REFLECT_BASE_TYPE(baseType) \
	entt::meta<baseType>().type(entt::hashed_string(#baseType)) \
		.func<&StringConverter::FromString<baseType>>(ReflectionGlobals::c_FromStringFunctionName);
