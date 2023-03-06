#include "Core.h"

#include "CoreComponents\DrawableComponents.h"

namespace ecs
{
	// Authoring data

	REFLECT_COMPONENT_BEGIN_DEFAULT(ModelComponent)
		REFLECT_COMPONENT_PROPERTY(ModelComponent::m_MeshPath, "MeshPath")
		REFLECT_COMPONENT_PROPERTY(ModelComponent::m_PixelShaderPath, "PixelShaderPath")
		REFLECT_COMPONENT_PROPERTY(ModelComponent::m_VertexShaderPath, "VertexShaderPath")
		REFLECT_COMPONENT_PROPERTY(ModelComponent::m_TexturePath, "TexturePath")
	REFLECT_COMPONENT_END(ModelComponent)

	REFLECT_COMPONENT_BEGIN_DEFAULT(SpriteComponent)
		REFLECT_COMPONENT_PROPERTY(SpriteComponent::m_ImagePath, "ImagePath")
		REFLECT_COMPONENT_PROPERTY(SpriteComponent::m_PixelShaderPath, "PixelShaderPath")
		REFLECT_COMPONENT_PROPERTY(SpriteComponent::m_VertexShaderPath, "VertexShaderPath")
	REFLECT_COMPONENT_END(SpriteComponent)

	// Runtime data

	REFLECT_COMPONENT_BEGIN_DEFAULT(BufferComponent)
	REFLECT_COMPONENT_END(BufferComponent)

	REFLECT_COMPONENT_BEGIN_DEFAULT(ShaderComponent)
	REFLECT_COMPONENT_END(ShaderComponent)

	REFLECT_COMPONENT_BEGIN_DEFAULT(TextureComponent)
	REFLECT_COMPONENT_END(TextureComponent)
} // namespace ecs