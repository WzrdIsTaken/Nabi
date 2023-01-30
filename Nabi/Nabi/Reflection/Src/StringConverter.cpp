#include "EngineCore.h"

#include "StringConverter.h"

#include "entt.h"

#include "DebugUtils.h"
#include "ReflectionGlobals.h"

namespace nabi::Reflection::StringConverter
{
	void ConvertFromString(std::string const& propertyValue, entt::hashed_string const& propertyTypeHash, entt::meta_any& metaObject, entt::meta_data const& metaMember) NABI_NOEXCEPT
	{
		/* 
		   Ok so strings are kind of an edge case, and so I am going to just hide this little block of logic here...
		   Why are they an edge case? Because "when you register FromString<std::string> on the meta type of std::string, 
	       it's treated as a non-static meta data member. This is due to the fact that you're registering a meta function for 
		   std::string that also accepts a reference to an std::string as its first argument. More or less the definition of 
		   member functions in C++" - Thank you very much skypjack for debugging that one more me! I would have never have guessed...

		   So, why has it been resolved in this way? Because it works, and a game engine is not just the reflection system - 
		   the plan is to get an MVP of all the systems I need, then come back and improve them if needed. 
		   I don't have infinite time... ): 
		*/

		if (entt::resolve<std::string>() == metaMember.type())
		{
			metaMember.set(metaObject, propertyValue);
			return;
		}

		entt::meta_type const dataType = entt::resolve(propertyTypeHash);
		if (entt::meta_func const fromString = dataType.func(ReflectionGlobals::c_FromStringFunctionName))
		{
			if (fromString.is_static())
			{
				entt::meta_any const result = fromString.invoke(dataType, propertyValue);
				metaMember.set(metaObject, result);
			}
			else
			{
				ASSERT_FAIL("The FromString method on " << propertyTypeHash.data() << " is not static!");
			}
		}
		else
		{
			ASSERT_FAIL("The type " << propertyTypeHash.data() << " does not have a FromString method!");
		}
	}

	std::string ExtractTypeName(std::string_view const typeInfoName) NABI_NOEXCEPT
	{
		std::string extractedTypeName = std::string(typeInfoName);
		ExtractTypeName(extractedTypeName);

		return extractedTypeName;
	}

	void ExtractTypeName(std::string& typeInfoName) NABI_NOEXCEPT
	{
		size_t const lastColon = typeInfoName.find_last_of(':') + 1; // i.e type name could look like MyNameSpace::MyType
		typeInfoName = typeInfoName.substr(lastColon, typeInfoName.size());
	}
} // namespace nabi::Reflection::StringConverter
