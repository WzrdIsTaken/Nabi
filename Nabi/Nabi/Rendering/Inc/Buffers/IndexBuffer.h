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
	struct IndexBuffer
	{
		wrl::ComPtr<ID3D11Buffer> m_Buffer;
	};

	class IndexBufferLoader
	{
	public:
		IndexBuffer operator()(Mesh const& mesh, nabi::Context const& context);
	};
} // namespace nabi::Rendering
