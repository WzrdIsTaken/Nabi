#pragma once
#include "Core.h"

namespace ecs
{
	// This component is assigned to entities in the xml (authoring data)

	struct ModelComponent final : public nabi::ECS::ComponentBase
	{
		std::string m_MeshPath;
		std::string m_PixelShaderPath;
		std::string m_VertexShaderPath;
		std::string m_TexturePath;
	};

	// It is then converted to these components (runtime data)

	struct MeshComponent final : public nabi::ECS::ComponentBase
	{
		unsigned int m_MeshId;
	};

	struct ShaderComponent final : public nabi::ECS::ComponentBase
	{
		unsigned int m_VertexShaderId;
		unsigned int m_PixelShaderId;
	};

	struct TextureComponent final : public nabi::ECS::ComponentBase
	{
		unsigned int m_TextureId;
	};
} // namespace ecs
