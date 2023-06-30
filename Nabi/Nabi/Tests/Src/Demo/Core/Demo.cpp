#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "Demo\Core\Demo.h"

#include "Demo\ECS\Modules\DemoModule.h"

namespace core
{
	Demo::Demo(HINSTANCE const hInstance, nabi::NabiCoreSettings const& initSettings)
		: nabi::NabiCore(hInstance, initSettings)
		, m_AssetBank(std::make_unique<DemoAssetBank>(m_Context))
	{
	}

	int Demo::Init()
	{
		int result = nabi::NabiCore::Init();
		
		ecs::DemoModule::StartDemo(m_Context);
		result &= static_cast<int>(m_AssetBank->LoadAssets());

		return result;
	}
} // namespace core

#endif // ifdef INCLUDE_DEMO
