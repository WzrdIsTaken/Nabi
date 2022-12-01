#include "BaseTypeReflection.h"

#include "../Libraries/entt/entt.h"
#include "StringConverter.h"
#include "ReflectionGlobals.h"
#include "ReflectionMacros.h"

namespace nabi::Reflection
{
	BaseTypeReflection::BaseTypeReflector BaseTypeReflection::baseTypeReflectorInstance;

	BaseTypeReflection::BaseTypeReflector::BaseTypeReflector()
	{
		REFLECT_BASE_TYPE(char)
		REFLECT_BASE_TYPE(int);
		REFLECT_BASE_TYPE(float);
		REFLECT_BASE_TYPE(double);
		REFLECT_BASE_TYPE(bool);
		REFLECT_BASE_TYPE(std::string);
	}
} // namespace nabi::Reflection
