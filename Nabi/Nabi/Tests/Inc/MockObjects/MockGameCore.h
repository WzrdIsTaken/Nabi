#pragma once
#include "Core.h"

//#include "NabiCore.h"

#ifdef RUN_TESTS

namespace nabitest
{
	class MockGameCore final //: public nabi::NabiCore
	{
	public:
		//MockGameCore() NABI_NOEXCEPT = default;
		//~MockGameCore() override = default;

		//int Init() override { return NABI_SUCCESS; }
		//int Run() override { return NABI_SUCCESS; }

	private:
		DELETE_COPY_MOVE_CONSTRUCTORS(MockGameCore)
	};
} // namespace nabitest

#endif // #ifdef RUN_TESTS
