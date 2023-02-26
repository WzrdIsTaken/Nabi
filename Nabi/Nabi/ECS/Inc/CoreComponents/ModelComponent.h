#pragma once
#include "Core.h"

#include "Model\Mesh.h"
#include "ResourceWrappers.h"
#include "Shaders\PixelShader.h"
#include "Shaders\VertexShader.h"
#include "Textures\Texture.h"

namespace ecs
{
	// This component is assigned to entities in the xml (authoring data)

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

	// It is then converted to these components (runtime data)

	struct MeshComponent final : public nabi::ECS::ComponentBase
	{
		nabi::Resource::ResourceRef<nabi::Rendering::Mesh> m_MeshResource;

		MeshComponent()
			: m_MeshResource{}
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
