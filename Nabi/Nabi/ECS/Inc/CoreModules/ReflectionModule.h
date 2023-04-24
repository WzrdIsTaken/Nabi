#pragma once
#include "Core.h"

namespace ecs::ReflectionModule
{
	struct Constraints
	{
		bool m_ExpectStatic;
		bool m_ExpectConst;
	};

	template<typename... Args>
	entt::meta_any CallReflectedFunction(entt::hashed_string const type, entt::hashed_string const function, Constraints const* const constraints, Args&&... args)
	{
		// TODO - Can StringConverter::ConvertFromString use this? The reason its not currently using this is because this logic was written a while after StringConverter.
		// But... its also kinda good that the ecs/StringConverter are seperated. Doesn't make a big difference either way I guess.

		entt::meta_any result = {};
		entt::meta_type const reflectedType = entt::resolve(type);
		entt::meta_func const reflectedFunc = reflectedType.func(function);

		if (reflectedFunc)
		{
			bool functionValid = true;
			ASSERT_CODE
			(
				if (constraints)
				{
					if (constraints->m_ExpectStatic && !reflectedFunc.is_static())
					{
						functionValid = false;
					}
					if (constraints->m_ExpectConst && !reflectedFunc.is_const())
					{
						functionValid = false;
					}
				}
			)

			if (functionValid)
			{
				result = reflectedFunc.invoke(reflectedType, std::forward<Args>(args)...);
			}
			else
			{
				ASSERT_FAIL("The reflected function didn't meet the constraints");
			}
		}
		else
		{
			ASSERT_FAIL("The type " << type.data() << " does not have a " << function.data() << " method!");
		}

		return result;
	}
} // namespace ecs::ReflectionModule
