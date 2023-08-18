#include "Core.h"

#include "MockObjects/MockCustomDataType.h"

#include "ReflectionIncludes.h"

#ifdef RUN_TESTS

namespace nabitest
{
	REFLECT_DATA_TYPE_DEFAULT(MockCustomDataType)
} // namespace nabitest::MockCustomDataType

#endif // #ifdef RUN_TESTS
