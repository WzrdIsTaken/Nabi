#include "../../Core/EngineCore.h"

#include "../Inc/BaseTypeReflection.h"

#include "../../Libraries/Inc/entt/entt.h"
#include "../Inc/StringConverter.h"
#include "../Inc/ReflectionGlobals.h"
#include "../Inc/ReflectionMacros.h"

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
