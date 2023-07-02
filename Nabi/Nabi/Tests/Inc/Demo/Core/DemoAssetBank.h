#pragma once
#include "Demo\DemoCore.h"

// honestly can just copy/paste this for real projects? perhaps some slight changes if i can think of better ways to do things..
// Notes:
// - LoadAssets should take in AssetType bitfield and switch off the enum
// - There should also be a RefreshAssets function (see Demo.cpp::RefreshLoadedAssets comment) which does a similar thing

#ifdef INCLUDE_DEMO

#include "AssetBank.h"
#include "AudioEffect.h"
#include "Buffers\RenderBuffers.h"
#include "Shaders\PixelShader.h"
#include "Shaders\VertexShader.h"
#include "ResourceBank.h"
#include "Textures\Texture.h"

namespace core
{
	enum class AssetType : uint64_t
	{
		Model  = 1 << 1,
		Sprite = 1 << 2,
		Text   = 1 << 3,
		Audio  = 1 << 4,

		All = ~0
	};
	DEFINE_ENUM_FLAG_OPERATORS(AssetType)

	class DemoAssetBank final : public nabi::Resource::AssetBank
	{
	public:
		DemoAssetBank(nabi::Context& context);
		~DemoAssetBank() override;

		bool LoadAssets() override;
		bool UnloadAssets() override;

	private:
		DELETE_COPY_MOVE_CONSTRUCTORS(DemoAssetBank)

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
