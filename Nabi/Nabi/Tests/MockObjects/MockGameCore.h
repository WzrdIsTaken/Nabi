#pragma once

#include "../../Core/NabiCore.h"

#ifdef _DEBUG

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

#endif // #ifdef _DEBUG
