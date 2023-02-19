#pragma once
#include "DirectXCore.h"

#include <vector> // perhaps should include engine core in these shader/texture/buffer files..?

namespace nabi::Rendering
{
	struct PixelShader final
	{
		wrl::ComPtr<ID3D11PixelShader> m_Shader;
		std::vector<wrl::ComPtr<ID3D11Buffer>> m_ConstantBuffers;
	};
} // namespace nabi::Rendering
