#pragma once

#include "../../Core.h"

#ifdef _DEBUG

namespace nabitest::MockSystem
{
	class MockSystem : public nabi::ECS::SystemBase
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
} // namespace nabitest::MockSystem

#endif // #ifdef _DEBUG
