#pragma once

#include "../../Core/NabiCore.h"

#ifdef RUN_TESTS

namespace nabitest
{
	class MockGameCore final : public nabi::NabiCore
	{
	public:
		MockGameCore() = default;
		~MockGameCore() override = default;

		void Init() override
		{
		}
		
		void Run() override
		{
		}

	private:
		MockGameCore(MockGameCore const&) = delete;
		MockGameCore& operator = (MockGameCore const&) = delete;
	};
} // namespace nabitest

#endif // #ifdef RUN_TESTS
