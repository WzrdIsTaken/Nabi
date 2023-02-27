#pragma once
#include "DirectXCore.h"

// Forward Declares
namespace nabi
{
	struct Context;
} // namespace nabi
namespace nabi::Rendering
{
	struct MeshData;
} // namespace nabi::Rending

namespace nabi::Rendering
{
	struct IndexBuffer final
	{
		wrl::ComPtr<ID3D11Buffer> m_Buffer;
	};

	class IndexBufferLoader final
	{
	public:
		[[nodiscard]] IndexBuffer operator()(MeshData const& meshData, nabi::Context const& context) const NABI_NOEXCEPT;
	};
} // namespace nabi::Rendering
