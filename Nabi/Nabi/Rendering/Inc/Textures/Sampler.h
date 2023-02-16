#pragma once
#include "DirectXCore.h"

namespace nabi::Rendering
{
	struct Sampler
	{
		wrl::ComPtr<ID3D11SamplerState> m_Sampler;
		//UINT m_Slot; could be used to identify samplers if we have more than one of them?
	};
} // namespace nabi::Rendering
