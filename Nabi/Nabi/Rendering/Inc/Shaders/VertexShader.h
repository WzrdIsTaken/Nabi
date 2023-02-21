#pragma once
#include "EngineCore.h"
#include "DirectXCore.h"

namespace nabi
{
	struct Context;
}

namespace nabi::Rendering
{
	inline std::vector<D3D11_INPUT_ELEMENT_DESC> const c_MeshInputLayout // Perhaps this could be reworked into an array?
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	struct VertexShader final
	{
		wrl::ComPtr<ID3D11VertexShader> m_Shader;
		wrl::ComPtr<ID3D11InputLayout> m_Layout;
		std::vector<wrl::ComPtr<ID3D11Buffer>> m_ConstantBuffers;
	};

	class VertexShaderLoader final
	{
	public:
		typedef std::shared_ptr<VertexShader> ResourceType;

		[[nodiscard]] ResourceType operator()(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT;
	};
} // namespace nabi::Rendering
