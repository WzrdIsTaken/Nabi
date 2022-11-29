#include "BaseTypeReflection.h"

#include "../Libraries/entt/entt.h"
#include "StringConverter.h"
#include "ReflectionGlobals.h"

namespace nabi::Reflection
{
	BaseTypeReflection::BaseTypeReflector BaseTypeReflection::baseTypeReflectorInstance;

	BaseTypeReflection::BaseTypeReflector::BaseTypeReflector()
	{
		using namespace entt::literals;

		entt::meta<char>().type("char"_hs)
			.func<&StringConverter::FromString<char>>(ReflectionGlobals::c_FromStringFunctionName);

		entt::meta<int>().type("int"_hs)
			.func<&StringConverter::FromString<int>>(ReflectionGlobals::c_FromStringFunctionName);

		entt::meta<float>().type("float"_hs)
			.func<&StringConverter::FromString<float>>(ReflectionGlobals::c_FromStringFunctionName);

		entt::meta<double>().type("double"_hs)
			.func<&StringConverter::FromString<double>>(ReflectionGlobals::c_FromStringFunctionName);

		entt::meta<bool>().type("bool"_hs)
			.func<&StringConverter::FromString<bool>>(ReflectionGlobals::c_FromStringFunctionName);

		entt::meta<std::string>().type("std::string"_hs)
			.func<&StringConverter::FromString<std::string>>(ReflectionGlobals::c_FromStringFunctionName);
	}
} // namespace nabi::Reflection
