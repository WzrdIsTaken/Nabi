#pragma once

#include "../../../Core.h"

#include "../../../Core/Inc/NabiCore.h"

#ifdef RUN_TESTS

namespace nabitest
{
	class MockGameCore final : public nabi::NabiCore
	{
	public:
		MockGameCore() NABI_NOEXCEPT = default;
		~MockGameCore() override = default;

		void Init() override
		{
		}
		
		void Run() override
		{
		}

	private:
		DELETE_COPY_MOVE_CONSTRUCTORS(MockGameCore)
	};
} // namespace nabitest

#endif // #ifdef RUN_TESTS
