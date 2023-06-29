#pragma once
#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "NabiCore.h"

namespace core
{
	class Demo final : public nabi::NabiCore
	{
	public:
		using nabi::NabiCore::NabiCore;
		~Demo() override = default;

		[[nodiscard]] int Init() override;
	};
} // namespace core

#endif // ifdef INCLUDE_DEMO
