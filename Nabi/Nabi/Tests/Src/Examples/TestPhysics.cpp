#include "TestCore.h"

#include "Examples\TestPhysics.h"

#ifdef RUN_TESTS

namespace nabitest::Examples
{
	TestPhysics::TestPhysics(nabi::Context& context)
		: m_Context(context)
	{
	}

	bool TestPhysics::Init()
	{
		return false;
	}

	bool TestPhysics::Update()
	{
		return false;
	}

	bool TestPhysics::Render()
	{
		return false;
	}
} // namespace nabitest::Examples

#endif // ifdef RUN_TESTS
