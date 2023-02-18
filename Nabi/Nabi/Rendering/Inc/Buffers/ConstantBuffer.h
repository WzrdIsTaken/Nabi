#pragma once
#include "EngineCore.h"
#include "DirectXCore.h"

// Forward Declares
namespace nabi
{
	struct Context;
} // namespace nabi

namespace nabi::Rendering
{
	// The actual constant buffers

	struct PerFrame
	{
		dx::XMFLOAT4X4 m_ViewMatrix;
		dx::XMFLOAT4X4 m_ProjectionMatrix;
	};

	struct PerMesh
	{
		dx::XMFLOAT4X4 m_ModelMatrix;
	};
} // namespace nabi::Rendering

namespace nabi::Rendering
{
	/// <summary>
	/// The different types of constant buffers so we can store them in a collection later
	/// </summary>
	struct ConstantBufferIndex
	{
		enum Enum
		{
			PerFrame = 0u,
			PerMesh,
			ENUM_COUNT
		};
	};

	/// <summary>
	/// A constant buffer
	/// </summary>
	struct ConstantBuffer
	{
		wrl::ComPtr<ID3D11Buffer> m_Buffer;
		UINT m_ByteWidth;
	};

	/// <summary>
	/// Creates a constant buffer. Don't really need this, but its for consistency...
	/// </summary>
	class ConstantBufferLoader
	{
	public:
		[[nodiscard]] ConstantBuffer operator()(UINT const byteWidth, nabi::Context const& context) const NABI_NOEXCEPT;
	};
} // namespace nabi::Rendering

