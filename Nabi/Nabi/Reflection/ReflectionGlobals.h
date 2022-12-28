#pragma once

// A collection of variables used throughout the reflection system, so it makes sense to group them in one place

namespace nabi::Reflection::ReflectionGlobals
{
	entt::hashed_string constexpr c_FromStringFunctionName = entt::hashed_string("FromString");
	entt::hashed_string constexpr c_AssignFunctionName = entt::hashed_string("Assign");
} // namespace nabi::Reflection::ReflectionGlobals
