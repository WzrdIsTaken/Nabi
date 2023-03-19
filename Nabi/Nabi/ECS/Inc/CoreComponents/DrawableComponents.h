#pragma once
#include "Core.h"

#include "Containers\UVs.h"
#include "Drawables\RenderBuffers.h"
#include "ResourceWrappers.h"
#include "Shaders\PixelShader.h"
#include "Shaders\VertexShader.h"
#include "Textures\Texture.h"

namespace ecs
{
	// These components are assigned to entities in the xml (authoring data)

	struct ModelResourceComponent final : public nabi::ECS::ComponentBase
	{
		std::string m_MeshPath;
		std::string m_PixelShaderPath;
		std::string m_VertexShaderPath;
		std::string m_TexturePath;

		ModelResourceComponent()
			: m_MeshPath("InvalidPath")
			, m_PixelShaderPath("InvalidPath")
			, m_VertexShaderPath("InvalidPath")
			, m_TexturePath("InvalidPath")
		{
		}
	};

	struct SpriteResourceComponent final : public nabi::ECS::ComponentSettingsBase
	{
		std::string m_ImagePath;
		std::string m_PixelShaderPath;
		std::string m_VertexShaderPath;

		// For sprite sheets 
		nabi::Rendering::UVs m_UVs;

		SpriteResourceComponent()
			: m_ImagePath("InvalidPath")
			, m_PixelShaderPath("InvalidPath")
			, m_VertexShaderPath("InvalidPath")
			, m_UVs(nabi::Rendering::UV::c_DefaultSpriteUVs)
		{
		}
	};

	struct TextResourceComponent final : public nabi::ECS::ComponentSettingsBase
	{
		std::string m_FontPath; // This should be a texture atlas of the font to use
		std::string m_PixelShaderPath;
		std::string m_VertexShaderPath;

		std::string m_Content; // What the text will display
		dx::XMFLOAT2 m_CharacterSpace; // The space between characters
		int m_AsciiShift; // char - m_AsciiShift = position of the character in the texture atlas
		dx::XMINT2 m_TextureAtlas; // The size of the texture atlas defined in m_FontPath

		int m_CharacterPoolSize; // The max number of characters in the text

		TextResourceComponent()
			: m_FontPath("InvalidPath")
			, m_PixelShaderPath("InvalidPath")
			, m_VertexShaderPath("InvalidPath")
			, m_Content("")
			, m_CharacterSpace(0, 0)
			, m_AsciiShift(0)
			, m_TextureAtlas(0, 0)
			, m_CharacterPoolSize(16)
		{
		}
	};

	// They are then converted to these components (runtime data)

	// Core 
	struct BufferComponent final : public nabi::ECS::ComponentBase
	{
		nabi::Resource::ResourceRef<nabi::Rendering::RenderBuffers> m_BufferResource;
	};

	struct ShaderComponent final : public nabi::ECS::ComponentBase
	{
		nabi::Resource::ResourceRef<nabi::Rendering::PixelShader> m_PixelShaderResource;
		nabi::Resource::ResourceRef<nabi::Rendering::VertexShader> m_VertexShaderResource;
	};

	struct TextureComponent final : public nabi::ECS::ComponentBase
	{
		nabi::Resource::ResourceRef<nabi::Rendering::Texture> m_TextureResource;
	};

	// Specific 
	struct TextComponent
	{
		entt::hashed_string m_Content;
		std::vector<entt::entity> m_Characters;
		dx::XMFLOAT2 m_CharacterSpace;

		int m_AsciiShift;
		dx::XMINT2 m_TextureAtlas;

		int m_CharacterPoolSize;
		int m_ActiveInPool;
	};

	// Ideas? 
	/*
		Could add this to every drawable if needed to identify their types one day
		struct DrawableComponent final : public nabi::ECS::ComponentBase
		{
			enum class DrawableType : int
			{
				Mesh,
				Sprite
			};

			DrawableType m_DrawableType;
		};
	*/
} // namespace ecs
