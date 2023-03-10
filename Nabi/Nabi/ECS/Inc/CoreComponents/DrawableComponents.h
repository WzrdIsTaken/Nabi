#pragma once
#include "Core.h"

#include "Drawables\RenderBuffers.h"
#include "ResourceWrappers.h"
#include "Shaders\PixelShader.h"
#include "Shaders\VertexShader.h"
#include "Textures\Texture.h"

namespace ecs
{
	// These components are assigned to entities in the xml (authoring data)

	struct ModelComponent final : public nabi::ECS::ComponentBase
	{
		std::string m_MeshPath;
		std::string m_PixelShaderPath;
		std::string m_VertexShaderPath;
		std::string m_TexturePath;

		ModelComponent()
			: m_MeshPath("InvalidPath")
			, m_PixelShaderPath("InvalidPath")
			, m_VertexShaderPath("InvalidPath")
			, m_TexturePath("InvalidPath")
		{
		}
	};

	struct SpriteComponent final : public nabi::ECS::ComponentSettingsBase
	{
		std::string m_ImagePath;
		std::string m_PixelShaderPath;
		std::string m_VertexShaderPath;

		// For sprite sheets (the uv of the texture)
		dx::XMFLOAT2 m_U;
		dx::XMFLOAT2 m_V;

		SpriteComponent()
			: m_ImagePath("InvalidPath")
			, m_PixelShaderPath("InvalidPath")
			, m_VertexShaderPath("InvalidPath")
			, m_U(1, 1)
			, m_V(0, 0)
		{
		}
	};

	struct TextComponent final : public nabi::ECS::ComponentSettingsBase
	{
		std::string m_Content; // What the text will display
		std::string m_FontPath; // This should be a texture atlas of the font to use
		dx::XMFLOAT2 m_CharacterSpace; // The space between characters
		int m_AsciiShift; // char - m_AsciiShift = position of the character in the texture atlas
		dx::XMINT2 m_TextureAtlas; // The size of the texture atlas defined in m_FontPath

		TextComponent()
			: m_Content("")
			, m_FontPath("InvalidPath")
			, m_CharacterSpace(0, 0)
			, m_AsciiShift(0)
			, m_TextureAtlas(0, 0)
		{
		}
	};

	// They are then converted to these components (runtime data)

	struct BufferComponent final : public nabi::ECS::ComponentBase
	{
		nabi::Resource::ResourceRef<nabi::Rendering::RenderBuffers> m_BufferResource;

		BufferComponent()
			: m_BufferResource{}
		{
		}
	};

	struct ShaderComponent final : public nabi::ECS::ComponentBase
	{
		nabi::Resource::ResourceRef<nabi::Rendering::PixelShader> m_PixelShaderResource;
		nabi::Resource::ResourceRef<nabi::Rendering::VertexShader> m_VertexShaderResource;

		ShaderComponent()
			: m_PixelShaderResource{}
			, m_VertexShaderResource{}
		{
		}
	};

	struct TextureComponent final : public nabi::ECS::ComponentBase
	{
		nabi::Resource::ResourceRef<nabi::Rendering::Texture> m_TextureResource;

		TextureComponent()
			: m_TextureResource{}
		{
		}
	};
} // namespace ecs
