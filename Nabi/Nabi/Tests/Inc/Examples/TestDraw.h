#pragma once
#include "TestCore.h"

#include "IExample.h"

#include "AssetBank.h"
#include "CoreSystems\RenderSystem.h"
#include "CoreSystems\LightingSystem.h"
#include "Drawables\RenderBuffers.h"
#include "Shaders\PixelShader.h"
#include "Shaders\VertexShader.h"
#include "ResourceBank.h"
#include "Textures\Texture.h"

/*
* Notes:
* Remember to do this for all shaders: https://youtu.be/pfbWt1BnPIo?list=PLqCJpWy5Fohd3S7ICFXwUomYW0Wv67pDD&t=1550
* And General->HLSL Compiler-> change the 'Shader Type' to the type of the shader
*/

#ifdef RUN_TESTS

namespace nabitest::Examples
{
	class TestDraw final : public IExample
	{
	public:
		TestDraw(nabi::Context& context);

		bool Init() override;
		bool Update() override;
		bool Render() override;

	private:
		class TestAssetBank final : public nabi::Resource::AssetBank
		{
		public:
			TestAssetBank(nabi::Context& context);
			virtual ~TestAssetBank() override;

			virtual bool LoadAssets() override;
			virtual bool UnloadAssets() override;

		private:
			bool Load3DModels();
			bool Load2DSprites();
			bool LoadText();

			nabi::Resource::ResourceBank<nabi::Rendering::Mesh, nabi::Rendering::RenderBufferLoader, 2> m_RenderBufferBank;
			nabi::Resource::ResourceBank<nabi::Rendering::PixelShader, nabi::Rendering::PixelShaderLoader, 2> m_PixelShaderBank;
			nabi::Resource::ResourceBank<nabi::Rendering::VertexShader, nabi::Rendering::VertexShaderLoader, 2> m_VertexShaderBank;
			nabi::Resource::ResourceBank<nabi::Rendering::Texture, nabi::Rendering::TextureLoader, 2> m_TextureBank;
		};

		nabi::Context& m_Context;
		ecs::RenderSystem m_RenderSystem;
		ecs::LightingSystem m_LightingSystem;

		std::unique_ptr<TestAssetBank> m_AssetBank;
	};
} // namespace nabitest::Examples

#endif //#ifdef RUN_TESTS
