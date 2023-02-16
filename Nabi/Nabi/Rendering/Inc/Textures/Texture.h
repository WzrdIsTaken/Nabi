#pragma once
#include "DirectXCore.h"

namespace nabi::Rendering
{
	struct Texture
	{
		wrl::ComPtr<ID3D11ShaderResourceView> m_Texture;
		UINT m_Slot;
	};
} // namespace nabi::Rendering
