#pragma once
#include "EngineCore.h"
#include "DirectXCore.h"

namespace nabi
{
	struct Context;
}

namespace nabi::Rendering
{
	struct PixelShader final
	{
		wrl::ComPtr<ID3D11PixelShader> m_Shader;
		std::vector<wrl::ComPtr<ID3D11Buffer>> m_ConstantBuffers;
	};

	class PixelShaderLoader final
	{
	public:
		typedef std::shared_ptr<PixelShader> ResourceType;

		ResourceType operator()(std::string const& resourcePath, nabi::Context const& context)  const NABI_NOEXCEPT;
	};
} // namespace nabi::Rendering
