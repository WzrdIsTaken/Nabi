#include "MockSystem.h"

#ifdef RUN_TESTS

namespace nabitest::ECS
{
	REFLECT_SYSTEM_BEGIN(MockSystem, "MockSystem")
		RELFECT_SYSTEM_PROPERTY(MockSystem::m_MockSystemData, "MockSystemData")
	RELFECT_SYSTEM_END(MockSystem)
} // namespace nabitest::ECS

#endif // #ifdef RUN_TESTS
