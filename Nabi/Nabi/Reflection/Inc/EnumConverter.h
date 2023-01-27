#pragma once

#include "../../Core/EngineCore.h"

#include "../../Libraries/Inc/entt/entt.h"
#include "../../Utils/Inc/DebugUtils.h"

namespace nabi::Reflection::EnumConverter
{
	/// <summary>
	/// Turns a string enum name to its actual enum value. Just use the name, eg: "MyEnum::Value" X | "Value" O
	/// </summary>
	/// <typeparam name="T">The enum</typeparam>
	/// <param name="enumName">- The name of the enum value</param>
	/// <returns>The enum value</returns>
	template<typename T>
	[[nodiscard]] inline T StringToEnum(std::string const& enumName) NABI_NOEXCEPT
	{
		/*
			Why std::string const& not std::string_view ? Because const& is needed for the deserialization xml->object stuff to work.
			And having two functions one with string_view and one with a normal string makes entt confused...
			Tbh thats just given me a thought - would rewriting the reflection ToString stuff to use string_view fix the string parsing issue 
			(see StringConverter::ConvertFromString (cpp)) as now the functions first arg wouldn't be a std::string? maybe... 
			But the reflection works and I don't want to touch it right now :D I'm like 50% sure past Ben already tried it as well...
		*/

		entt::hashed_string const enumNameHash = entt::hashed_string(enumName.c_str());
		return entt::resolve<T>().data(enumNameHash).get({}).cast<T>();
	}

	// Soon tm!
	inline std::string EnumToString(int const /*_enum*/) NABI_NOEXCEPT
	{
		FUNCTION_NOT_IMPLEMENTED

		// Todo - Implement me! (If its ever needed...)
		return {};
	}
} // namespace nabi::Reflection::EnumConverter
