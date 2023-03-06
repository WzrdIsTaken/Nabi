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

		SpriteComponent()
			: m_ImagePath("InvalidPath")
			, m_PixelShaderPath("InvalidPath")
			, m_VertexShaderPath("InvalidPath")
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
