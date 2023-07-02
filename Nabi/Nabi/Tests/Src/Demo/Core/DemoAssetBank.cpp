#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "CoreComponents\ResourceComponents\AudioResourceComponent.h"
#include "CoreComponents\ResourceComponents\ModelResourceComponent.h"
#include "CoreComponents\ResourceComponents\SpriteResourceComponent.h"
#include "CoreComponents\ResourceComponents\TextResourceComponent.h"
#include "CoreComponents\BufferComponent.h"
#include "CoreComponents\ShaderComponent.h"
#include "CoreComponents\TextureComponent.h"

#include "Demo\Core\DemoAssetBank.h"

namespace core
{
#define REMOVE_RESOURCE_COMPONENT_FROM_ENTITY

	DemoAssetBank::DemoAssetBank(nabi::Context& context)
		: AssetBank(context)

		// Render Banks
		, m_RenderBufferBank(context)
		, m_VertexShaderBank(context)
		, m_PixelShaderBank(context)
		, m_TextureBank(context)

		// Audio Bank
		, m_AudioEffectBank(context)
	{
	}

	DemoAssetBank::~DemoAssetBank()
	{
		UnloadAssets();
	}

	bool DemoAssetBank::LoadAssets()
	{
		bool loadingSuccessful = true;
		loadingSuccessful &= LoadModels();
		loadingSuccessful &= LoadSprites();
		loadingSuccessful &= LoadText();
		loadingSuccessful &= LoadAudio();

		return loadingSuccessful;
	}

	bool DemoAssetBank::UnloadAssets()
	{
		m_RenderBufferBank.Clear();
		m_VertexShaderBank.Clear();
		m_PixelShaderBank.Clear();
		m_TextureBank.Clear();
		m_AudioEffectBank.Clear();

		return true;
	}

	bool DemoAssetBank::LoadModels()
	{
		SetRenderBankProperties(AssetType::Model);

		m_Context.m_Registry.view<ecs::RComp::ModelResourceComponent>()
			.each([&](entt::entity const entity, auto const& modelResourceComponent)
				{
					RenderablePaths const modelPaths =
					{
						modelResourceComponent.m_MeshPath,
						modelResourceComponent.m_VertexShaderPath,
						modelResourceComponent.m_PixelShaderPath,
						modelResourceComponent.m_TexturePath
					};
					LoadRenderable(modelPaths, entity, std::nullopt);

#ifdef REMOVE_RESOURCE_COMPONENT_FROM_ENTITY
					m_Context.m_Registry.remove<ecs::RComp::ModelResourceComponent>(entity);
#endif // ifdef REMOVE_RESOURCE_COMPONENT_FROM_ENTITY
				});

		return true;
	}

	bool DemoAssetBank::LoadSprites()
	{
		SetRenderBankProperties(AssetType::Sprite);
		FAST_LOG("Implement me!");

		// would it be too bad to make the renderable loading a macro ? cos the m_Mesh/Sprite path is different
		// for each i couldnt use a template to do the whole thing

		return true;
	}

	bool DemoAssetBank::LoadText()
	{
		SetRenderBankProperties(AssetType::Text);
		FAST_LOG("Implement me!");

		return true;
	}

	bool DemoAssetBank::LoadAudio()
	{
		FAST_LOG("Implement me!");

		return true;
	}

	void DemoAssetBank::SetRenderBankProperties(AssetType const renderableType)
	{
		using namespace nabi::Rendering;

		RenderBufferLoader& renderBufferLoader = m_RenderBufferBank.GetLoader();
		VertexShaderLoader& vertexShaderLoader = m_VertexShaderBank.GetLoader();
		PixelShaderLoader& pixelShaderLoader = m_PixelShaderBank.GetLoader();

		switch (renderableType)
		{
		case AssetType::Model:
			{
				renderBufferLoader.SetLoadMode(RenderBufferLoader::LoadMode::_3D);
				vertexShaderLoader.SetInputLayout(Layouts::c_MeshInputLayout);
				vertexShaderLoader.SetConstantBuffers({ ConstantBufferIndex::PerFrame, ConstantBufferIndex::PerRenderable });
				pixelShaderLoader.SetConstantBuffers({ ConstantBufferIndex::PerLightChange });
			}
			break;
		case AssetType::Sprite:
			[[fallthrough]]; // (Each text character is just a sprite)
		case AssetType::Text:
			{
				renderBufferLoader.SetLoadMode(RenderBufferLoader::LoadMode::_2D);
				vertexShaderLoader.SetInputLayout(Layouts::c_SpriteInputLayout);
				vertexShaderLoader.SetConstantBuffers({});
				pixelShaderLoader.SetConstantBuffers({});
			}
			break;
		default:
			ASSERT_FAIL("Using an unexpected RenderableType!");
			break;
		}
	}

	void DemoAssetBank::LoadRenderable(RenderablePaths const& renderablePaths, entt::entity const entity, std::optional<std::function<void()>> preLoadOperation)
	{
		using namespace nabi::Rendering;
		using namespace nabi::Resource;

		ecs::BufferComponent bufferComponent = {};
		ecs::ShaderComponent shaderComponent = {};
		ecs::TextureComponent textureComponent = {};

		if (preLoadOperation)
		{
			(*preLoadOperation)();
		}

		// Asset
		ResourceRef<RenderBuffers> const renderBufferResource = m_RenderBufferBank.LoadResource(renderablePaths.m_AssetPath);
		bufferComponent.m_BufferResource = renderBufferResource;

		// Shaders
		ResourceRef<PixelShader> const pixelShaderResource = m_PixelShaderBank.LoadResource(renderablePaths.m_PixelShaderPath);
		ResourceRef<VertexShader> const vertexShaderResource = m_VertexShaderBank.LoadResource(renderablePaths.m_VertexShaderPath);
		shaderComponent.m_PixelShaderResource = pixelShaderResource;
		shaderComponent.m_VertexShaderResource = vertexShaderResource;

		// Texture
		ResourceRef<Texture> const textureResource = m_TextureBank.LoadResource(renderablePaths.m_TexturePath);
		textureComponent.m_TextureResource = textureResource;

		// Add components to entity
		m_Context.m_Registry.emplace_or_replace<ecs::BufferComponent>(entity, bufferComponent);
		m_Context.m_Registry.emplace_or_replace<ecs::ShaderComponent>(entity, shaderComponent);
		m_Context.m_Registry.emplace_or_replace<ecs::TextureComponent>(entity, textureComponent);
	}
} // namespace core

#endif // ifdef INCLUDE_DEMO
