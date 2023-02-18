#pragma once
#include "TestCore.h"

// Examples are kinda like me testing stuff out, rather than an automated unit test ran on project startup

namespace nabitest::Examples
{
	__interface IExample
	{
		bool Init(nabi::Context& context);
		bool Update();
		bool Render();
	};
} // namespace nabitest::Examples
