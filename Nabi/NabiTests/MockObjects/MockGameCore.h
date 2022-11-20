#pragma once

#include "Core/NabiCore.h"

namespace nabitests
{
	class MockGameCore : public nabi::NabiCore
	{
	public:
		MockGameCore() = default;
		~MockGameCore() = default;

		void Init() override
		{
		}
		// TODO: RULE OF 6
		void Run() override
		{
		}

	private:
		MockGameCore(MockGameCore const&) = delete;
		MockGameCore& operator = (MockGameCore const&) = delete;
	};
} // namespace nabitests
