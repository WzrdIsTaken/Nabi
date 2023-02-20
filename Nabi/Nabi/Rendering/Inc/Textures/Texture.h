#pragma once
#include "EngineCore.h"
#include "DirectXCore.h"

namespace nabi
{
	struct Context;
}

namespace nabi::Rendering
{
	struct Texture
	{
		wrl::ComPtr<ID3D11ShaderResourceView> m_Texture;
		UINT m_Slot;
	};

	class TextureLoader final
	{
	public:
		typedef std::shared_ptr<Texture> ResourceType;

		ResourceType operator()(std::string const& resourcePath, nabi::Context const& context) const NABI_NOEXCEPT;
	};
} // namespace nabi::Rendering
