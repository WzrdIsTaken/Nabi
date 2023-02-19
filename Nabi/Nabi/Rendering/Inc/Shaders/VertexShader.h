#pragma once
#include "DirectXCore.h"

#include <vector> // perhaps should include engine core in these shader/texture/buffer files..?

namespace nabi::Rendering
{
	struct VertexShader final
	{
		wrl::ComPtr<ID3D11VertexShader> m_Shader;
		wrl::ComPtr<ID3D11InputLayout> m_Layout;
		std::vector<wrl::ComPtr<ID3D11Buffer>> m_ConstantBuffers;
	};
} // namespace nabi::Rendering
