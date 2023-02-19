#pragma once
#include "DirectXCore.h"

// Forward Declares
namespace nabi
{
	struct Context;
} // namespace nabi
namespace nabi::Rendering
{
	struct Mesh;
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
		[[nodiscard]] IndexBuffer operator()(Mesh const& mesh, nabi::Context const& context) const NABI_NOEXCEPT;
	};
} // namespace nabi::Rendering
