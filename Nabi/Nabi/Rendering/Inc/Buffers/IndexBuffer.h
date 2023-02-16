#pragma once
#include "DirectXCore.h"

namespace nabi::Rendering
{
	struct IndexBuffer
	{
		wrl::ComPtr<ID3D11Buffer> m_Buffer;
	};
} // namespace nabi::Rendering
