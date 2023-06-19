#include "TestCore.h"

#include "Examples\TestThreading.h"

#ifdef RUN_TESTS

namespace nabitest::Examples
{
	TestThreading::TestThreading(nabi::Context& context)
		: m_Context(context)
	{
	}

	TestThreading::~TestThreading()
	{
	}

	bool TestThreading::Init()
	{
		return false;
	}

	bool TestThreading::Update()
	{
		return false;
	}

	bool TestThreading::Render()
	{
		return false;
	}
} // namespace nabitest::Examples

#endif // ifdef RUN_TESTS
