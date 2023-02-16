#pragma once
#include "DirectXCore.h"

namespace nabi::Rendering
{
	struct ConstantBuffer
	{
		wrl::ComPtr<ID3D11Buffer> m_Buffer;
		UINT m_ByteWidth;
	};
} // namespace nabi::Rendering
