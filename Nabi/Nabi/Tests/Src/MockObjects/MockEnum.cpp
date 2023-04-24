#include "Core.h"

#include "MockObjects/MockEnum.h"

#ifdef RUN_TESTS

namespace nabitest
{
	REFLECT_ENUM_BEGIN_DEFAULT(MockEnum)
		REFLECT_ENUM_VALUE(MockEnum::None, "None")
		REFLECT_ENUM_VALUE_DEFAULT(MockEnum::Ben)
		REFLECT_ENUM_VALUE_DEFAULT(MockEnum::Kyra)
		REFLECT_ENUM_VALUE_DEFAULT(MockEnum::AnotherEntry)
		REFLECT_ENUM_VALUE_DEFAULT(MockEnum::ENUM_COUNT)
	REFLECT_ENUM_END(MockEnum)

	namespace NamespacedMockEnum
	{
		REFLECT_ENUM_BEGIN_DEFAULT(Enum)
			REFLECT_ENUM_VALUE_DEFAULT(Enum::TestOne)
			REFLECT_ENUM_VALUE_DEFAULT(Enum::TestOne)
		REFLECT_ENUM_END(Enum)
	} // namespace NamespacedMockEnum
} // namespace nabitest

#endif // ifdef RUN_TESTS
