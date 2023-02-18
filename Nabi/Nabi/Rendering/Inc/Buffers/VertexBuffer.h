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
	struct VertexBuffer
	{
		wrl::ComPtr<ID3D11Buffer> m_Buffer;
		UINT m_Stride;
		UINT m_Offset;
	};

	class VertexBufferLoader
	{
	public:
		VertexBuffer operator()(Mesh const& mesh, nabi::Context const& context);
	};
} // namespace nabi::Rendering
