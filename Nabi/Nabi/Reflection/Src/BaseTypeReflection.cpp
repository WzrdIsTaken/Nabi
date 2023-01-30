#include "EngineCore.h"

#include "BaseTypeReflection.h"

#include "entt.h"

#include "ReflectionGlobals.h"
#include "ReflectionMacros.h"
#include "StringConverter.h"

namespace nabi::Reflection
{
	BaseTypeReflection::BaseTypeReflector BaseTypeReflection::s_BaseTypeReflectorInstance;

	BaseTypeReflection::BaseTypeReflector::BaseTypeReflector() NABI_NOEXCEPT
	{
		REFLECT_BASE_TYPE(char)
		REFLECT_BASE_TYPE(int);
		REFLECT_BASE_TYPE(float);
		REFLECT_BASE_TYPE(double);
		REFLECT_BASE_TYPE(bool);
		REFLECT_BASE_TYPE(std::string);
	}
} // namespace nabi::Reflection
