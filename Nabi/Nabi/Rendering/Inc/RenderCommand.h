#pragma once
#include "EngineCore.h"
#include "WinCore.h"
#include "DirectXCore.h"

#include "Buffers/ConstantBuffer.h"
#include "Buffers/IndexBuffer.h"
#include "Buffers/VertexBuffer.h"
#include "Containers/Colour.h"
#include "Model/Vertex.h"
#include "Shaders/PixelShader.h"
#include "Shaders/VertexShader.h"
#include "Textures/Sampler.h"
#include "Textures/Texture.h"

// Forward Declares
namespace nabi
{
	struct WindowSettings;
}

namespace nabi::Rendering
{
	// (more!) Forward Declares
	struct ConstantBuffer;
	struct DXObjects;

	/// <summary>
	/// A basic cache so we don't bind unnecessary
	/// Its usage can be toggled on/off with USE_RENDER_CACHE (defined in the cpp)
	/// </summary>
	struct RenderCommandCache final
	{
		ID3D11VertexShader* m_LastBoundVertexShader;
		ID3D11PixelShader* m_LastBoundPixelShader;

		ID3D11Buffer* m_LastBoundVertexBuffer;
		ID3D11Buffer* m_LastBoundIndexBuffer;

		ID3D11ShaderResourceView* m_LastBoundTexture;
		ID3D11SamplerState* m_LastBoundSampler;

		RenderCommandCache() NABI_NOEXCEPT
			: m_LastBoundVertexShader(nullptr)
			, m_LastBoundPixelShader(nullptr)
			, m_LastBoundVertexBuffer(nullptr)
			, m_LastBoundIndexBuffer(nullptr)
			, m_LastBoundTexture(nullptr)
			, m_LastBoundSampler(nullptr)
		{
		}
	};

	/// <summary>
	/// The RenderCommand talks to DirectX to do all things rendering.
	/// Apparently a RenderCommand is actually something different and this should be called like 'RenderCommandCentre' - but 'RenderCommand' sounds cooler :D
	/// </summary>
	class RenderCommand final
	{
	public:
		// --- Initialization ---

		RenderCommand(DXObjects& dxObjects, HWND const hWnd, WindowSettings const& settings) NABI_NOEXCEPT;

		// --- Frame ---

		void BeginFrame() const NABI_NOEXCEPT;
		void EndFrame() const NABI_NOEXCEPT;

		// --- Back Buffer ---

		void Clear() const NABI_NOEXCEPT;
		void Swap() const NABI_NOEXCEPT;
		void SetBackgroundColour(Colour const& colour) NABI_NOEXCEPT;

		// --- Creation ---

		ConstantBuffer CreateConstantBuffer(UINT const byteWidth) const NABI_NOEXCEPT;
		IndexBuffer CreateIndexBuffer(std::vector<UINT> const& triangles) const NABI_NOEXCEPT;
		VertexBuffer CreateVertexBuffer(std::vector<Vertex> const& vertices) const NABI_NOEXCEPT;

		PixelShader CreatePixelShader(std::string const& filePath) const NABI_NOEXCEPT;
		VertexShader CreateVertexShader(std::string const& filePath, std::vector<D3D11_INPUT_ELEMENT_DESC> const& layout) const NABI_NOEXCEPT;

		Texture CreateTexture(std::string const& filePath) const NABI_NOEXCEPT;
		Sampler CreateSampler() const NABI_NOEXCEPT;

		// --- Binding ---

		void BindPixelShader(PixelShader const& pixelShader) NABI_NOEXCEPT;
		void BindVertexShader(VertexShader const& vertexShader) NABI_NOEXCEPT;

		void BindIndexBuffer(IndexBuffer const& indexBuffer) NABI_NOEXCEPT;
		void BindVertexBuffer(VertexBuffer const& vertexBuffer) NABI_NOEXCEPT;

		void BindTexture(Texture const& texture) NABI_NOEXCEPT;
		void BindSampler(Sampler const& sampler) NABI_NOEXCEPT;

		// --- Updating ---

		void UpdateConstantBuffer(ConstantBuffer const& constantBuffer, void const* const data) const NABI_NOEXCEPT;

		// --- Drawing ---

		void Draw(UINT const indexCount) const NABI_NOEXCEPT;
		void DrawIndexed(UINT const indexCount) const NABI_NOEXCEPT;

		// --- Getters ---

		RenderCommandCache const& GetCache() const NABI_NOEXCEPT;
		Colour GetBackgroundColour() const NABI_NOEXCEPT;

		// --- Setters ---

		void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY const primitiveTopology) NABI_NOEXCEPT;

	private:
		DELETE_COPY_MOVE_CONSTRUCTORS(RenderCommand)

		DXObjects& m_DXObjects;
		RenderCommandCache m_Cache;
		std::array<FLOAT, 4> m_BackgroundColour;
	};
} // namespace nabi::Rendering
