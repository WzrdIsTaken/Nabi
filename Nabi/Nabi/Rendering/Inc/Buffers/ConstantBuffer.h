#pragma once
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
	/// Though the fact that it takes in a UINT as a first arg rather than a sting makes it inconsistent xD
	/// </summary>
	class ConstantBufferLoader
	{
	public:
		ConstantBuffer operator()(UINT const byteWidth, nabi::Context const& context);
	};
} // namespace nabi::Rendering

