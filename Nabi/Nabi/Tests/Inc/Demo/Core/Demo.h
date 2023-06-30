#pragma once
#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "NabiCore.h"

#include "DemoAssetBank.h"

namespace nabi
{
	struct NabiCoreSettings;
} // namespace nabi

namespace core
{
	class Demo final : public nabi::NabiCore
	{
	public:
		Demo(HINSTANCE const hInstance, nabi::NabiCoreSettings const& initSettings);
		~Demo() override = default;

		[[nodiscard]] int Init() override;

	private:
		std::unique_ptr<DemoAssetBank> m_AssetBank;
	};
} // namespace core

#endif // ifdef INCLUDE_DEMO
