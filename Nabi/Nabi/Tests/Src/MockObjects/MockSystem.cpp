#include "Core.h"

#include "MockObjects/MockSystem.h"

#ifdef RUN_TESTS

namespace nabitest::ECS
{
	REFLECT_SYSTEM_BEGIN(MockSystem, "MockSystem")
		REFLECT_SYSTEM_PROPERTY(MockSystem::m_MockSystemData, "MockSystemData")
	RELFECT_SYSTEM_END(MockSystem)
} // namespace nabitest::ECS

#endif // #ifdef RUN_TESTS
