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
} // namespace nabitest::ECS

#endif // #ifdef RUN_TESTS
