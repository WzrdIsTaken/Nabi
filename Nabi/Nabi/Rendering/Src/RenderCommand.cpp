#include "EngineCore.h"

#include "RenderCommand.h"

#include "WICTextureLoader.h"

#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib") // Could do this in project settings perhaps
#pragma comment(lib, "D3DCompiler.lib") // ""

#include "Containers\Colour.h"
#include "DebugUtils.h"
#include "DXObjects.h"
#include "InitSettings.h"
#include "StringUtils.h"

namespace nabi::Rendering
{
#define USE_RENDER_CACHE

	RenderCommand::RenderCommand(DXObjects& dxObjects, HWND const hWnd, WindowSettings const& settings) NABI_NOEXCEPT
		: m_DXObjects(dxObjects)
		, m_Cache{}
		, m_BackgroundColour{ 0, 0, 0, 0 }
	{
		// Create swap chan descriptor
		DXGI_SWAP_CHAIN_DESC swapChainDescriptor{};
		swapChainDescriptor.BufferDesc.Width = 0;
		swapChainDescriptor.BufferDesc.Height = 0;
		swapChainDescriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDescriptor.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDescriptor.BufferDesc.RefreshRate.Denominator = 0;
		swapChainDescriptor.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDescriptor.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDescriptor.SampleDesc.Count = 1;
		swapChainDescriptor.SampleDesc.Quality = 0;
		swapChainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDescriptor.BufferCount = 2; // Double buffering
		swapChainDescriptor.OutputWindow = hWnd;
		swapChainDescriptor.Windowed = TRUE;
		swapChainDescriptor.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDescriptor.Flags = 0;

		UINT createDeviceFlags = 0;
#ifdef USE_DEBUG_UTILS
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // ifdef USE_DEBUG_UTILS

		// Create device, front/back buffers, swap chain and rendering context 
		DX_ASSERT(D3D11CreateDeviceAndSwapChain(
			nullptr,				  // Adapter 
			D3D_DRIVER_TYPE_HARDWARE, // Driver Type
			nullptr,                  // Software
			createDeviceFlags,        // Flags
			nullptr,                  // Feature Levels
			0u,                       // Feature Levels
			D3D11_SDK_VERSION,        // SDK Version
			&swapChainDescriptor,     // Swap Chain Descriptor
			&m_DXObjects.m_SwapChain, // Swap Chain
			&m_DXObjects.m_Device,    // Device
			nullptr,                  // Feature Level
			&m_DXObjects.m_Context	  // Device Context
		));

#ifdef USE_DEBUG_UTILS
		DX_ASSERT(m_DXObjects.m_Device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_DXObjects.m_DebugDevice)));
		DX_ASSERT(m_DXObjects.m_DebugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL));
#endif // ifdef USE_DEBUG_UTILS

		// Gain access to texture subresource in swap chain (back buffer)
		wrl::ComPtr<ID3D11Resource> backBuffer;
		DX_ASSERT(m_DXObjects.m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
		DX_ASSERT(m_DXObjects.m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_DXObjects.m_RenderTargetView));

		// Create the depth stencil state
		wrl::ComPtr<ID3D11DepthStencilState> depthStencilState;
		D3D11_DEPTH_STENCIL_DESC depthStencilDescriptor = {};
		depthStencilDescriptor.DepthEnable = true;
		depthStencilDescriptor.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDescriptor.DepthFunc = D3D11_COMPARISON_LESS;

		DX_ASSERT(m_DXObjects.m_Device->CreateDepthStencilState(&depthStencilDescriptor, &depthStencilState));

		// Bind the depth stencil state to the pipeline
		m_DXObjects.m_Context->OMSetDepthStencilState(depthStencilState.Get(), 1u);

		// Create the depth stensil texture
		wrl::ComPtr<ID3D11Texture2D> depthStencil;
		D3D11_TEXTURE2D_DESC depthDescriptor = {};
		depthDescriptor.Width = static_cast<UINT>(settings.m_Width);
		depthDescriptor.Height = static_cast<UINT>(settings.m_Height);
		depthDescriptor.MipLevels = 1u;
		depthDescriptor.ArraySize = 1u;
		depthDescriptor.Format = DXGI_FORMAT_D32_FLOAT;
		depthDescriptor.SampleDesc.Count = 1u;
		depthDescriptor.SampleDesc.Quality = 0u;
		depthDescriptor.Usage = D3D11_USAGE_DEFAULT;
		depthDescriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		DX_ASSERT(m_DXObjects.m_Device->CreateTexture2D(&depthDescriptor, nullptr, &depthStencil));

		// Create a view of the depth stencil texture
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDescriptor = {};
		depthStencilViewDescriptor.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilViewDescriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDescriptor.Texture2D.MipSlice = 0u;

		DX_ASSERT(m_DXObjects.m_Device->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDescriptor, &m_DXObjects.m_DepthStencilView));

		// Set the primitive topology (D3D11_PRIMITIVE_TOPOLOGY_LINELIST is useful for debugging)
		m_DXObjects.m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Setup and bind viewport
		m_DXObjects.m_Viewport.TopLeftX = 0.0f;
		m_DXObjects.m_Viewport.TopLeftY = 0.0f;
		m_DXObjects.m_Viewport.Width = static_cast<FLOAT>(settings.m_Width);
		m_DXObjects.m_Viewport.Height = static_cast<FLOAT>(settings.m_Height);
		m_DXObjects.m_Viewport.MinDepth = 0.0f;
		m_DXObjects.m_Viewport.MaxDepth = 1.0f;

		m_DXObjects.m_Context->RSSetViewports(1u, &m_DXObjects.m_Viewport);

		// Set the background colour. Only right its cornflower blue...
		SetBackgroundColour(Colours::CornflowerBlue);
	}

	void RenderCommand::BeginFrame() const NABI_NOEXCEPT
	{
		m_DXObjects.m_Context->OMSetRenderTargets(1u, &m_DXObjects.m_RenderTargetView, m_DXObjects.m_DepthStencilView.Get()); // Bind the depth stencil view to ON
		Clear();
	}

	void RenderCommand::EndFrame() const NABI_NOEXCEPT
	{
		Swap();
	}

	void RenderCommand::Clear() const NABI_NOEXCEPT
	{
		m_DXObjects.m_Context->ClearRenderTargetView(m_DXObjects.m_RenderTargetView /*.Get()*/, m_BackgroundColour.data());
		m_DXObjects.m_Context->ClearDepthStencilView(m_DXObjects.m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}

	void RenderCommand::Swap() const NABI_NOEXCEPT
	{
		DX_ASSERT(m_DXObjects.m_SwapChain->Present(1u, 0u));
	}

	void RenderCommand::SetBackgroundColour(Colour const& colour) NABI_NOEXCEPT
	{
		m_BackgroundColour = colour.ToArray<FLOAT>(Colour::ColourRange::O_TO_1);
	}

	ConstantBuffer RenderCommand::CreateConstantBuffer(UINT const byteWidth) const NABI_NOEXCEPT
	{
		ASSERT_FATAL(std::fmodf(static_cast<float>(byteWidth), 16.0f) == 0, 
			"Constant buffer's size must be divisible by 16! The buffers size was " << byteWidth);

		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = byteWidth;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		ID3D11Buffer* buffer = nullptr;
		DX_ASSERT(m_DXObjects.m_Device->CreateBuffer(&bufferDesc, nullptr, &buffer));

		ConstantBuffer constantBuffer = {};
		constantBuffer.m_Buffer = buffer;
		constantBuffer.m_ByteWidth = byteWidth;

		LOG(LOG_PREP, LOG_INFO, "Created a constant buffer of size " << bufferDesc.ByteWidth << ENDLINE);
		return constantBuffer;
	}

	IndexBuffer RenderCommand::CreateIndexBuffer(std::vector<UINT> const& triangles) const NABI_NOEXCEPT
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.CPUAccessFlags = 0u; // CPU doesn't need to access the buffer (yet!)
		bufferDesc.MiscFlags = 0u;
		bufferDesc.ByteWidth = static_cast<UINT>(triangles.size() * sizeof(UINT));
		bufferDesc.StructureByteStride = sizeof(UINT);

		D3D11_SUBRESOURCE_DATA subresourceData = {};
		subresourceData.pSysMem = triangles.data();

		ID3D11Buffer* buffer = nullptr;
		DX_ASSERT(m_DXObjects.m_Device->CreateBuffer(&bufferDesc, &subresourceData, &buffer));

		IndexBuffer indexBuffer = {};
		indexBuffer.m_Buffer = buffer;

		LOG(LOG_PREP, LOG_INFO, "Created an index buffer of size " << bufferDesc.ByteWidth << ENDLINE);
		return indexBuffer;
	}

	VertexBuffer RenderCommand::CreateVertexBuffer(std::vector<Vertex> const& vertices) const NABI_NOEXCEPT
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.CPUAccessFlags = 0u;
		bufferDesc.MiscFlags = 0u;
		bufferDesc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * std::size(vertices));
		bufferDesc.StructureByteStride = sizeof(Vertex);

		D3D11_SUBRESOURCE_DATA subresourceData = {};
		subresourceData.pSysMem = vertices.data(); // Vertices to use

		ID3D11Buffer* buffer = nullptr;
		DX_ASSERT(m_DXObjects.m_Device->CreateBuffer(&bufferDesc, &subresourceData, &buffer));

		VertexBuffer vertexBuffer = {};
		vertexBuffer.m_Buffer = buffer;
		vertexBuffer.m_Stride = sizeof(Vertex);
		vertexBuffer.m_Offset = 0u;

		LOG(LOG_PREP, LOG_INFO, "Created a vertex buffer of size " << bufferDesc.ByteWidth << ENDLINE);
		return vertexBuffer;
	}

	PixelShader RenderCommand::CreatePixelShader(std::string const& filePath) const NABI_NOEXCEPT
	{
		ID3D11PixelShader* pixelShader = nullptr;
		Microsoft::WRL::ComPtr<ID3DBlob> blob;

		LPCWSTR filePathAsLpcwstr;
		STRING_TO_LPCWSTR(filePath, filePathAsLpcwstr);

		DX_ASSERT(D3DReadFileToBlob(filePathAsLpcwstr, &blob));
		DX_ASSERT(m_DXObjects.m_Device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader));

		PixelShader shader = {};
		shader.m_Shader = pixelShader;

		LOG(LOG_PREP, LOG_INFO, "Created a pixel shader with path " << WRAP(filePath, "'") << ENDLINE);
		return shader;
	}

	VertexShader RenderCommand::CreateVertexShader(std::string const& filePath, std::vector<D3D11_INPUT_ELEMENT_DESC> const& layout) const NABI_NOEXCEPT
	{
		ID3D11VertexShader* vertexShader = nullptr;
		Microsoft::WRL::ComPtr<ID3DBlob> blob;

		LPCWSTR filePathAsLpcwstr;
		STRING_TO_LPCWSTR(filePath, filePathAsLpcwstr);

		DX_ASSERT(D3DReadFileToBlob(filePathAsLpcwstr, &blob));
		DX_ASSERT(m_DXObjects.m_Device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader));

		ID3D11InputLayout* inputLayout = nullptr;
		DX_ASSERT(m_DXObjects.m_Device->CreateInputLayout(layout.data(), static_cast<UINT>(layout.size()), blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout));

		VertexShader shader = {};
		shader.m_Shader = vertexShader;
		shader.m_Layout = inputLayout;

		LOG(LOG_PREP, LOG_INFO, "Created a vertex shader with path " << WRAP(filePath, "'") << ENDLINE);
		return shader;
	}

	Texture RenderCommand::CreateTexture(std::string const& filePath) const NABI_NOEXCEPT
	{
		LPCWSTR filePathAsLpcwstr;
		STRING_TO_LPCWSTR(filePath, filePathAsLpcwstr);

		wrl::ComPtr<ID3D11ShaderResourceView> shaderResourceView = nullptr;
		DX_ASSERT(dx::CreateWICTextureFromFile(m_DXObjects.m_Device.Get(), filePathAsLpcwstr, NULL /*(texture view)*/, &shaderResourceView));

		Texture texture = {};
		texture.m_Texture = shaderResourceView;
		texture.m_Slot = 0u;

		LOG(LOG_PREP, LOG_INFO, "Created a texture with path " << WRAP(filePath, "'") << ENDLINE);
		return texture;
	}

	Sampler RenderCommand::CreateSampler() const NABI_NOEXCEPT
	{
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		ID3D11SamplerState* samplerState = nullptr;
		DX_ASSERT(m_DXObjects.m_Device->CreateSamplerState(&samplerDesc, &samplerState));

		Sampler sampler = {};
		sampler.m_Sampler = samplerState;

		LOG(LOG_PREP, LOG_INFO, "Created a sampler" << ENDLINE);
		return sampler;
	}

	void RenderCommand::BindPixelShader(PixelShader const& pixelShader) NABI_NOEXCEPT
	{
#ifdef USE_RENDER_CACHE
		if (m_Cache.m_LastBoundPixelShader != pixelShader.m_Shader.Get())
#endif // ifdef USE_RENDER_CACHE
		{
			m_Cache.m_LastBoundPixelShader = pixelShader.m_Shader.Get();

			m_DXObjects.m_Context->PSSetShader(
				pixelShader.m_Shader.Get(), // Pixel Shader
				nullptr,                    // Class Instances
				0u                          // Number of Class Instances
			);
			m_DXObjects.m_Context->PSSetConstantBuffers(
				0u,                                                  // Start Slot
				pixelShader.m_ConstantBuffers.size(),                // Number of Constant Nummbers
				pixelShader.m_ConstantBuffers.data()->GetAddressOf() // Constant Buffer(s)
			);
		}
	}

	void RenderCommand::BindVertexShader(VertexShader const& vertexShader) NABI_NOEXCEPT
	{
#ifdef USE_RENDER_CACHE
		if (m_Cache.m_LastBoundVertexShader != vertexShader.m_Shader.Get())
#endif // ifdef USE_RENDER_CACHE
		{
			m_Cache.m_LastBoundVertexShader = vertexShader.m_Shader.Get();
			m_DXObjects.m_Context->IASetInputLayout(vertexShader.m_Layout.Get());

			m_DXObjects.m_Context->VSSetShader(
				vertexShader.m_Shader.Get(), // Vertex Shader
				nullptr,                     // Class Instances
				0u                           // Number of Class Instances
			);
			m_DXObjects.m_Context->VSSetConstantBuffers(
				0u,                                                   // Start Slot
				vertexShader.m_ConstantBuffers.size(),                // Number of Constant Buffers
				vertexShader.m_ConstantBuffers.data()->GetAddressOf() // Constant Buffer(s)
			);
		}
	}

	void RenderCommand::BindIndexBuffer(IndexBuffer const& indexBuffer) NABI_NOEXCEPT
	{
#ifdef USE_RENDER_CACHE
		if (m_Cache.m_LastBoundIndexBuffer != indexBuffer.m_Buffer.Get())
#endif // ifdef USE_RENDER_CACHE
		{
			m_Cache.m_LastBoundIndexBuffer = indexBuffer.m_Buffer.Get();
			m_DXObjects.m_Context->IASetIndexBuffer(
				indexBuffer.m_Buffer.Get(), // Index Buffer
				DXGI_FORMAT_R32_UINT,       // Format
				0u                          // Offset
			);
		}
	}

	void RenderCommand::BindVertexBuffer(VertexBuffer const& vertexBuffer) NABI_NOEXCEPT
	{
#ifdef USE_RENDER_CACHE
		if (m_Cache.m_LastBoundVertexBuffer != vertexBuffer.m_Buffer.Get())
#endif // ifdef USE_RENDER_CACHE
		{
			m_Cache.m_LastBoundVertexBuffer = vertexBuffer.m_Buffer.Get();

			m_DXObjects.m_Context->IASetVertexBuffers(
				0u,                                   // Start Slot
				1u,                                   // Num Buffers
				vertexBuffer.m_Buffer.GetAddressOf(), // Vertex Buffer(s)
				&vertexBuffer.m_Stride,               // Strides
				&vertexBuffer.m_Offset                // Offset
			);
		}
	}

	void RenderCommand::BindTexture(Texture const& texture) NABI_NOEXCEPT
	{
#ifdef USE_RENDER_CACHE
		if (m_Cache.m_LastBoundTexture != texture.m_Texture.Get())
#endif // ifdef USE_RENDER_CACHE
		{
			m_Cache.m_LastBoundTexture = texture.m_Texture.Get();
			m_DXObjects.m_Context->PSSetShaderResources(
				texture.m_Slot,                    // Start Slot
				1u,                                // Num Views
				texture.m_Texture.GetAddressOf()); // Shader Resource View(s)
		}
	}

	void RenderCommand::BindSampler(Sampler const& sampler) NABI_NOEXCEPT
	{
#ifdef USE_RENDER_CACHE
		if (m_Cache.m_LastBoundSampler != sampler.m_Sampler.Get())
#endif // ifdef USE_RENDER_CACHE
		{
			m_Cache.m_LastBoundSampler = sampler.m_Sampler.Get();
			m_DXObjects.m_Context->PSSetSamplers(
				0u,                              // Start Slot
				1u,                              // Number of samplers
				sampler.m_Sampler.GetAddressOf() // Sampler(s)
			);
		}
	}

	void RenderCommand::UpdateConstantBuffer(ConstantBuffer const& constantBuffer, void const* const data) const NABI_NOEXCEPT
	{
		D3D11_MAPPED_SUBRESOURCE subresource;
		DX_ASSERT(m_DXObjects.m_Context->Map(
			constantBuffer.m_Buffer.Get(), // Resource
			0u,                            // Subresource
			D3D11_MAP_WRITE_DISCARD,       // Map type
			0u,                            // Map Flags
			&subresource                   // Mapped Resource
		));

		memcpy(subresource.pData, data, constantBuffer.m_ByteWidth);
		m_DXObjects.m_Context->Unmap(
			constantBuffer.m_Buffer.Get(), // Resource
			0u                             // Subresource
		);
	}

	void RenderCommand::Draw(UINT const indexCount) const NABI_NOEXCEPT
	{
		m_DXObjects.m_Context->Draw(
			indexCount, // Index Count
			0u          // Start Vertex Location
		);
	}

	void RenderCommand::DrawIndexed(UINT const indexCount) const NABI_NOEXCEPT
	{
		m_DXObjects.m_Context->DrawIndexed(
			indexCount, // Index Count
			0u,         // Start Index Location
			0           // Base Vertex Location
		);
	}

	RenderCommandCache const& RenderCommand::GetCache() const NABI_NOEXCEPT
	{
		return m_Cache;
	}

	Colour RenderCommand::GetBackgroundColour() const NABI_NOEXCEPT
	{
		return Colour::FromArray(m_BackgroundColour);
	}

	void RenderCommand::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY const primitiveTopology) NABI_NOEXCEPT
	{
		m_DXObjects.m_Context->IASetPrimitiveTopology(primitiveTopology);
	}
} // namespace nabi::Rendering
