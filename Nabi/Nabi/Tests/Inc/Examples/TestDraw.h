#pragma once
#include "TestCore.h"

#include "IExample.h"

#include "AssetBank.h"
#include "CoreSystems\RenderSystem.h"
#include "Model\Mesh.h"
#include "Shaders\PixelShader.h"
#include "Shaders\VertexShader.h"
#include "ResourceBank.h"
#include "Textures\Texture.h"

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

			nabi::Resource::ResourceBank<nabi::Rendering::Mesh, nabi::Rendering::MeshLoader, 1> m_MeshBank;
			nabi::Resource::ResourceBank<nabi::Rendering::PixelShader, nabi::Rendering::PixelShaderLoader, 1> m_PixelShaderBank;
			nabi::Resource::ResourceBank<nabi::Rendering::VertexShader, nabi::Rendering::VertexShaderLoader, 1> m_VertexShaderBank;
			nabi::Resource::ResourceBank<nabi::Rendering::Texture, nabi::Rendering::TextureLoader, 1> m_TextureBank;
		};

		nabi::Context& m_Context;
		ecs::RenderSystem m_RenderSystem;

		std::unique_ptr<TestAssetBank> m_AssetBank;
	};
} // namespace nabitest::Examples

#endif //#ifdef RUN_TESTS
