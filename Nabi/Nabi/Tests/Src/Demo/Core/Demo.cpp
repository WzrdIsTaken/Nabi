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
		result &= static_cast<int>(m_AssetBank->LoadAssets());

		return result;
	}

	void Demo::RefreshLoadedAssets() const
	{
		/*
		  In a real project, this function should take in a enum bitfield and asset bank will only refresh 
		  assets which match the bitfield. See notes section at the top of DemoAssetBank.h

		  Eg:

		  auto constexpr assetsToRefresh = core::AssetType::Mesh;
		  RefreshLoadedAssets(assetsToRefresh);

		  LoadAssets() should also be wrapped in an assert.

		  The use case for this is that:
			- Entity templates are created with the resource components
			- When they are created as part of a group, those components need to be replaced with the real thing
				- Eg: ModelResourceComponent -> Buffer/Shader/Texture component
			- This will be fast because the resources have already been loaded.
		*/

		m_AssetBank->LoadAssets();
	}
} // namespace core

#endif // ifdef INCLUDE_DEMO
