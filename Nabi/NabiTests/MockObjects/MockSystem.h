#pragma once

#include "Core.h"
#include "ECS/SystemBase.h"

namespace nabitests::MockSystem
{
	class MockSystem : nabi::ECS::SystemBase
	{
	public:
		using SystemBase::SystemBase;

		int GetMockSystemData() const
		{
			return m_MockSystemData;
		}

	private:
		MockSystem(MockSystem const&) = delete;
		MockSystem& operator = (MockSystem const&) = delete;

		int m_MockSystemData;

		REFLECT_PRIVATES(MockSystem)
	};

	REFLECT_SYSTEM_BEGIN(MockSystem, "MockSystem")
		REFLECT_SYSTEM_METHOD(MockSystem::m_MockSystemData, "MockSystemData")
	RELFECT_SYSTEM_END()
} // namespace nabitests::MockSystem
