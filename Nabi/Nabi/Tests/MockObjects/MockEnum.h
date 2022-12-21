#pragma once

#include "../../Core.h"

#ifdef _DEBUG

namespace nabitest
{
	enum class MockEnum 
	{
		None,
		Ben  = 20,
		Kyra = 27,
		AnotherEntry,
		ENUM_COUNT
	};

	REFLECT_ENUM_BEGIN_DEFAULT(MockEnum)
		REFLECT_ENUM_VALUE(MockEnum::None, "None")
		REFLECT_ENUM_VALUE_DEFAULT(MockEnum::Ben)
		REFLECT_ENUM_VALUE_DEFAULT(MockEnum::Kyra)
		REFLECT_ENUM_VALUE_DEFAULT(MockEnum::AnotherEntry)
		REFLECT_ENUM_VALUE_DEFAULT(MockEnum::ENUM_COUNT)
	REFLECT_ENUM_END
} // namespace nabitest

#endif // ifdef _DEBUG
