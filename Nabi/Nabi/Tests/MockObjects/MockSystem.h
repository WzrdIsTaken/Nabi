#pragma once

#include "../../Core.h"

#ifdef RUN_TESTS

namespace nabitest::ECS
{
	class MockSystem final : public nabi::ECS::SystemBase
	{
	public:
		using SystemBase::SystemBase;

		int GetMockSystemData() const
		{
			return m_MockSystemData;
		}

	private:
		int m_MockSystemData;

		REFLECT_PRIVATES(MockSystem)
	};

	REFLECT_SYSTEM_BEGIN(MockSystem, "MockSystem")
		RELFECT_SYSTEM_PROPERTY(MockSystem::m_MockSystemData, "MockSystemData")
	RELFECT_SYSTEM_END()
} // namespace nabitest::ECS

#endif // #ifdef RUN_TESTS
