#pragma once
#include "EngineCore.h"

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
			ReflectorConstructor() NABI_NOEXCEPT \
			{ \
				InitReflection(); \
			} 

#define CREATE_REFLECTOR_END(typeName) \
		}; \
		\
		static ReflectorConstructor s_ReflectedConstructor; \
	}; \
	CREATE_REFLECTOR_INSTANCE(typeName)

#define CREATE_REFLECTOR_INSTANCE(typeName) CONCAT(typeName, Reflector::ReflectorConstructor) CONCAT(typeName, Reflector::s_ReflectedConstructor);

// --- Helper ---
// A couple of helper macros to do operations which are done multiple times in multiple places.
// The REFLECT_PRIVATES macro just friends the Reflector class, so it can access (normally systems, as they are classes) properties for reflection.

#define CREATE_INIT_REFLECTION_FUNCTION() \
	private: \
	static void InitReflection() NABI_NOEXCEPT \
	{

#define REFLECT_PRIVATES(typeName) \
	friend class CONCAT(typeName, Reflector);

// --- Component Reflection ---
// The assign function for components emplaces the component to the passed in entity in the registry.
// As for the general flow of reflection, we begin, reflect the properties and end. Begin/end just create the reflector class. 
// The DEFAULT variation of the macros stringifys the type name.

// Assign function
#define CREATE_ASSIGN_FUNCTION(componentType) \
	static void Assign(entt::meta_any const& metaComponent, entt::registry& registry, entt::entity const entity) NABI_NOEXCEPT \
	{ \
		using T = componentType; \
		T const component = metaComponent.cast<T>(); \
		\
		registry.emplace<T>(entity, component); \
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
#define REFLECT_COMPONENT_END(component) \
			.func<&Assign>(nabi::Reflection::ReflectionGlobals::c_AssignFunctionName); \
	} \
	CREATE_REFLECTOR_END(component)

// --- Data Type Reflection ---
// Data type (/just normal structs and classes) reflection works a bit differently to systems and components. 
// Their type just needs to be registered with the reflection system. However - note that all custom types need to define a FromString function.

#define REFLECT_DATA_TYPE_DEFAULT(dataType) \
	CREATE_REFLECTOR_BEGIN(dataType) \
	CREATE_INIT_REFLECTION_FUNCTION() \
		entt::meta<dataType>().type(entt::hashed_string(#dataType)) \
			.func<&dataType::FromString>(nabi::Reflection::ReflectionGlobals::c_FromStringFunctionName); \
	} \
	CREATE_REFLECTOR_END(dataType)

#define REFLECT_DATA_TYPE(dataType, reflectedName) \
	CREATE_REFLECTOR_BEGIN(dataType) \
	CREATE_INIT_REFLECTION_FUNCTION() \
		entt::meta<dataType>().type(entt::hashed_string(reflectedName)) \
			.func<&dataType::FromString>(nabi::Reflection::ReflectionGlobals::c_FromStringFunctionName); \
	} \
	CREATE_REFLECTOR_END(dataType)

// --- System Reflection ---
// System reflection works in exactly the same way as component reflection for a user.
// However under the hood the Assign method is not added to the Reflector class because it is not needed.
// Also, note - System constructors must take in a reference to the registery, a const system id and a const system group id.

// Begin
#define REFLECT_SYSTEM_BEGIN_DEFAULT(system) \
	CREATE_REFLECTOR_BEGIN(system) \
	CREATE_INIT_REFLECTION_FUNCTION() \
		entt::meta<system>().type(entt::hashed_string(#system)) \
			.ctor<nabi::Context&, entt::hashed_string const, entt::hashed_string const>()

#define REFLECT_SYSTEM_BEGIN(system, reflectedName) \
	CREATE_REFLECTOR_BEGIN(system) \
	CREATE_INIT_REFLECTION_FUNCTION() \
		entt::meta<system>().type(entt::hashed_string(reflectedName)) \
			.ctor<nabi::Context&, entt::hashed_string const, entt::hashed_string const>()

// Functions
#define REFLECT_SYSTEM_METHOD_DEFAULT(method) \
			.func<&method>(entt::hashed_string(#method))

#define REFLECT_SYSTEM_METHOD(method, reflectedName) \
			.func<&method>(entt::hashed_string(reflectedName))

// Properties
#define REFLECT_SYSTEM_PROPERTY_DEFAULT(member) \
			.data<&member>(entt::hashed_string(#member))

#define REFLECT_SYSTEM_PROPERTY(member, reflectedName) \
			.data<&member>(entt::hashed_string(reflectedName))

// End
#define RELFECT_SYSTEM_END(system) \
	; } \
	CREATE_REFLECTOR_END(system)

// --- Enum Reflection ---
// A macro to handle the reflection of enums. Works in basically the same way as the others user facing wise.
// However, to get the enum reflected data (eg, enum value -> string name) use the functions defined in EnumConverter.
// The StringToEnum function defined in EnumConverter is also used for the xml->object FromString function.

#define REFLECT_ENUM_BEGIN(_enum, enumName) \
	CREATE_REFLECTOR_BEGIN(_enum) \
	CREATE_INIT_REFLECTION_FUNCTION() \
		entt::meta<_enum>().type(entt::hashed_string(enumName)) \
			.func<&nabi::Reflection::EnumConverter::StringToEnum<_enum>>(nabi::Reflection::ReflectionGlobals::c_FromStringFunctionName)

#define REFLECT_ENUM_BEGIN_DEFAULT(_enum) \
	CREATE_REFLECTOR_BEGIN(_enum) \
	CREATE_INIT_REFLECTION_FUNCTION() \
		entt::meta<_enum>().type(entt::hashed_string(#_enum)) \
			.func<&nabi::Reflection::EnumConverter::StringToEnum<_enum>>(nabi::Reflection::ReflectionGlobals::c_FromStringFunctionName)

#define REFLECT_ENUM_VALUE(enumValue, enumValueName) \
			.data<enumValue>(entt::hashed_string(enumValueName))

#define REFLECT_ENUM_VALUE_DEFAULT(enumValue) \
			.data<enumValue>(entt::hashed_string(nabi::Reflection::StringConverter::ExtractTypeName(#enumValue).c_str()))

#define REFLECT_ENUM_END(_enum) \
	; } \
	CREATE_REFLECTOR_END(_enum)
	
// --- Base Type Reflection ---
// A simple marcro to reflect base types. The ToString method for base types will always live on StringConverter.

#define REFLECT_BASE_TYPE(baseType) \
	entt::meta<baseType>().type(entt::hashed_string(#baseType)) \
		.func<&nabi::Reflection::StringConverter::FromString<baseType>>(nabi::Reflection::ReflectionGlobals::c_FromStringFunctionName);
