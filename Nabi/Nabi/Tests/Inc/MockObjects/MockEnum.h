#pragma once
#include "Core.h"

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

	enum class MockEnumFlags
	{
		One   = 1 << 1,
		Two   = 1 << 2,
		Three = 1 << 3,

		All   = ~0
	};
	DEFINE_ENUM_FLAG_OPERATORS(MockEnumFlags)

	namespace NamespacedMockEnum
	{
		enum Enum : int
		{
			TestOne,
			TestTwo
		};
	} // namespace NamespacedMockEnum
} // namespace nabitest

#endif // ifdef RUN_TESTS
