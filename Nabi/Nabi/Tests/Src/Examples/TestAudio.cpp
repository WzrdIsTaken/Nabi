#include "TestCore.h"

#include "Examples\TestAudio.h"

#ifdef RUN_TESTS

namespace nabitest::Examples
{
	TestAudio::TestAudio(nabi::Context& context)
		: m_Context(context)
	{
	}

	bool TestAudio::Init()
	{
		return false;
	}

	bool TestAudio::Update()
	{
		return false;
	}

	bool TestAudio::Render()
	{
		return false;
	}
} // namespace nabitest::Examples

#endif // ifdef RUN_TESTS
