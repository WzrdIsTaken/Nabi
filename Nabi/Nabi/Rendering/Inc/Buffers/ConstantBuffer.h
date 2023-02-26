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
	// The actual constant buffers (size must be divisible by 16 for DirectX)

	struct PerFrame final
	{
		dx::XMFLOAT4X4 m_ViewMatrix;
		dx::XMFLOAT4X4 m_ProjectionMatrix;
		dx::XMFLOAT3 m_CameraPosition;
		char /*const*/ c_Padding[4];
	};

	struct PerMesh final
	{
		dx::XMFLOAT4X4 m_ModelMatrix;
	};
} // namespace nabi::Rendering

namespace nabi::Rendering
{
	/// <summary>
	/// The different types of constant buffers so we can store them in a collection later
	/// </summary>
	struct ConstantBufferIndex final
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
	struct ConstantBuffer final
	{
		wrl::ComPtr<ID3D11Buffer> m_Buffer;
		UINT m_ByteWidth;
	};

	/// <summary>
	/// Creates a constant buffer. Don't really need this, but its for consistency...
	/// </summary>
	class ConstantBufferLoader final
	{
	public:
		enum class AddMode
		{
			ClearAndAdd,
			Append,
			ENUM_COUNT
		};

		[[nodiscard]] ConstantBuffer operator()(UINT const byteWidth, nabi::Context const& context) const NABI_NOEXCEPT;

		static void AssignConstantBuffersToShader(
			std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>>& shaderConstantBuffers, std::vector<ConstantBufferIndex::Enum> const& constantBuffersToAssign,
			AddMode const addMode, nabi::Context const& context) NABI_NOEXCEPT;
	};
} // namespace nabi::Rendering

