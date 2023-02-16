#pragma once
#include "DirectXCore.h"

namespace nabi::Rendering
{
	struct VertexBuffer
	{
		wrl::ComPtr<ID3D11Buffer> m_Buffer;
		UINT m_Stride;
		UINT const c_Offset = 0u;
	};
} // namespace nabi::Rendering
