#pragma once
#include "TestCore.h"

#include "IExample.h"

#ifdef RUN_TESTS

namespace nabitest::Examples
{
	class TestPhysics final : public IExample
	{
	public:
		TestPhysics(nabi::Context& context);

		bool Init() override;
		bool Update() override;
		bool Render() override;

	private:
		nabi::Context& m_Context;
	};
} // namespace nabitest::Examples

#endif // ifdef RUN_TESTS
