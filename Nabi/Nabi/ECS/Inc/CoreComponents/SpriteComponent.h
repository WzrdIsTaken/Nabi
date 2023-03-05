#pragma once
#include "Core.h"

#include "ResourceWrappers.h"
#include "Sprite\Sprite.h"

namespace ecs
{
	// This component is assigned to entities in the xml (authoring data)

	struct ImageComponent : public nabi::ECS::ComponentBase
	{
		std::string m_ImagePath;
		std::string m_PixelShaderPath;
		std::string m_VertexShaderPath;

		ImageComponent()
			: m_ImagePath("InvalidPath")
			, m_PixelShaderPath("InvalidPath")
			, m_VertexShaderPath("InvalidPath")
		{
		}
	};

	// It is then converted to these components (runtime data)

	struct SpriteComponent : public nabi::ECS::ComponentBase
	{
	};
} // namespace ecs
