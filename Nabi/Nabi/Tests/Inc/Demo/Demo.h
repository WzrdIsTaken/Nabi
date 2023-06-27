#pragma once
#include "DemoCore.h"

#include "NabiCore.h"

#ifdef INCLUDE_DEMO

namespace demo
{
	class Demo final : public nabi::NabiCore
	{
	public:
		using nabi::NabiCore::NabiCore;
		~Demo() override;

		int Init() override;
		int Run() override;
	};
} // namespace demo

#endif // ifdef INCLUDE_DEMO
