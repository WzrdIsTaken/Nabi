#pragma once

#include "../../../Core.h"

#ifdef RUN_TESTS

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
} // namespace nabitest

#endif // ifdef RUN_TESTS
