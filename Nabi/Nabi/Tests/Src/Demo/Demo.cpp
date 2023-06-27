#include "Demo\DemoCore.h"
#include "..\..\Inc\Demo\Demo.h"

#ifdef INCLUDE_DEMO

namespace demo
{
	Demo::~Demo()
	{
		nabi::NabiCore::~NabiCore();
	}

	int Demo::Init()
	{
		return nabi::NabiCore::Init();
	}

	int Demo::Run()
	{
		return nabi::NabiCore::Run();
	}
} // namespace demo

#endif // ifdef INCLUDE_DEMO
