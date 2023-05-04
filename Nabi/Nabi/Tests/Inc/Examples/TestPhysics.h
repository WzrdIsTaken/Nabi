#pragma once
#include "TestCore.h"

#include "IExample.h"

#include "AssetBank.h"
#include "ResourceBank.h"
#include "CoreSystems\PhysicsSystem.h"
#include "CoreSystems\RenderSystem.h"
#include "Buffers\RenderBuffers.h"
#include "Shaders\PixelShader.h"
#include "Shaders\VertexShader.h"
#include "Textures\Texture.h"

// TODO forward declare ^

#ifdef RUN_TESTS

namespace nabitest::Examples
{
	class TestPhysics final : public IExample
	{
	public:
		TestPhysics(nabi::Context& context);

		bool Init() override;
		bool Update() override;
		bool Render() override;

	private:
		struct SimpleAssetBank : nabi::Resource::AssetBank
		{
		public:
			SimpleAssetBank(nabi::Context& context);
			virtual ~SimpleAssetBank() override;

			virtual bool LoadAssets() override;
			virtual bool UnloadAssets() override;

		private:
			bool Load3DModels();

			nabi::Resource::ResourceBank<nabi::Rendering::Mesh, nabi::Rendering::RenderBufferLoader, 20> m_RenderBufferBank;
			nabi::Resource::ResourceBank<nabi::Rendering::PixelShader, nabi::Rendering::PixelShaderLoader, 20> m_PixelShaderBank;
			nabi::Resource::ResourceBank<nabi::Rendering::VertexShader, nabi::Rendering::VertexShaderLoader, 20> m_VertexShaderBank;
			nabi::Resource::ResourceBank<nabi::Rendering::Texture, nabi::Rendering::TextureLoader, 20> m_TextureBank;
		};

		nabi::Context& m_Context;

		std::unique_ptr<ecs::PhysicsSystem> m_PhysicsSystem;
		std::unique_ptr<ecs::RenderSystem> m_RenderSystem;

		std::unique_ptr<SimpleAssetBank> m_AssetBank;
	};
} // namespace nabitest::Examples

#endif // ifdef RUN_TESTS
