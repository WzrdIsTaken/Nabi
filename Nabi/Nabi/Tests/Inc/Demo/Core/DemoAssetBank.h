#pragma once
#include "Demo\DemoCore.h"

// honestly can just copy/paste this for real projects? perhaps some slight changes if i can think of better ways to do things..
// + probs should tidy it up a bit

#ifdef INCLUDE_DEMO

#include "AssetBank.h"
#include "AudioEffect.h"
#include "Buffers\RenderBuffers.h"
#include "Shaders\PixelShader.h"
#include "Shaders\VertexShader.h"
#include "ResourceBank.h"
#include "Textures\Texture.h"

#include "Demo\Core\AssetTypes.h"

namespace core
{
	class DemoAssetBank final : public nabi::Resource::AssetBank
	{
	public:
		DemoAssetBank(nabi::Context& context);
		~DemoAssetBank() override;

		bool LoadAssets(AssetType const assetType);
		bool UnloadAssets(); // unloading assets is currently only supported as a bulk operation

		inline bool RefreshLoadedAssets(AssetType const assetType) { return LoadAssets(assetType); };

	private:
		DELETE_COPY_MOVE_CONSTRUCTORS(DemoAssetBank)

		constexpr inline bool ValidAssetType(AssetType const assetTypeOne, AssetType const assetTypeTwo) const noexcept 
		{ 
			return static_cast<bool>(assetTypeOne & assetTypeTwo);
		}

		bool LoadModels();
		bool LoadSprites();
		bool LoadText();
		bool LoadAudio();

#pragma region Render Banks
		static size_t constexpr c_RenderBankCapacity = 5u;

		struct RenderablePaths final
		{
			std::string const& m_AssetPath;
			std::string const& m_VertexShaderPath;
			std::string const& m_PixelShaderPath;
			std::string const& m_TexturePath;
		};

		void SetRenderBankProperties(AssetType const renderableType);
		void LoadRenderable(RenderablePaths const& renderablePaths, entt::entity const entity, std::optional<std::function<void()>> preLoadOperation);

		nabi::Resource::ResourceBank<nabi::Rendering::Mesh,         nabi::Rendering::RenderBufferLoader, c_RenderBankCapacity> m_RenderBufferBank;
		nabi::Resource::ResourceBank<nabi::Rendering::VertexShader, nabi::Rendering::VertexShaderLoader, c_RenderBankCapacity> m_VertexShaderBank;
		nabi::Resource::ResourceBank<nabi::Rendering::PixelShader,  nabi::Rendering::PixelShaderLoader,  c_RenderBankCapacity> m_PixelShaderBank;
		nabi::Resource::ResourceBank<nabi::Rendering::Texture,      nabi::Rendering::TextureLoader,      c_RenderBankCapacity> m_TextureBank;
#pragma endregion

#pragma region Audio Bank
		static size_t constexpr c_AudioBankCapacity = 5u;

		nabi::Resource::ResourceBank<nabi::Audio::AudioEffect, nabi::Audio::AudioEffectLoader, c_AudioBankCapacity> m_AudioEffectBank;
#pragma endregion
	};
} // namespace core

#endif // ifdef INCLUDE_DEMO
