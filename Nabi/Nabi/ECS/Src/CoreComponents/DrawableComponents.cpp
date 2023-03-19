#include "Core.h"

#include "CoreComponents\DrawableComponents.h"

namespace ecs
{
	// Authoring data

	REFLECT_COMPONENT_BEGIN_DEFAULT(ModelResourceComponent)
		REFLECT_COMPONENT_PROPERTY(ModelResourceComponent::m_MeshPath, "MeshPath")
		REFLECT_COMPONENT_PROPERTY(ModelResourceComponent::m_PixelShaderPath, "PixelShaderPath")
		REFLECT_COMPONENT_PROPERTY(ModelResourceComponent::m_VertexShaderPath, "VertexShaderPath")
		REFLECT_COMPONENT_PROPERTY(ModelResourceComponent::m_TexturePath, "TexturePath")
	REFLECT_COMPONENT_END(ModelResourceComponent)

	REFLECT_COMPONENT_BEGIN_DEFAULT(SpriteResourceComponent)
		REFLECT_COMPONENT_PROPERTY(SpriteResourceComponent::m_ImagePath, "ImagePath")
		REFLECT_COMPONENT_PROPERTY(SpriteResourceComponent::m_PixelShaderPath, "PixelShaderPath")
		REFLECT_COMPONENT_PROPERTY(SpriteResourceComponent::m_VertexShaderPath, "VertexShaderPath")
		REFLECT_COMPONENT_PROPERTY(SpriteResourceComponent::m_UVs, "UVs")
	REFLECT_COMPONENT_END(SpriteResourceComponent)

	REFLECT_COMPONENT_BEGIN_DEFAULT(TextResourceComponent)
		REFLECT_COMPONENT_PROPERTY(TextResourceComponent::m_FontPath, "FontPath")
		REFLECT_COMPONENT_PROPERTY(TextResourceComponent::m_PixelShaderPath, "PixelShaderPath")
		REFLECT_COMPONENT_PROPERTY(TextResourceComponent::m_VertexShaderPath, "VertexShaderPath")
		REFLECT_COMPONENT_PROPERTY(TextResourceComponent::m_Content, "Content")
		REFLECT_COMPONENT_PROPERTY(TextResourceComponent::m_CharacterSpace, "CharacterSpace")
		REFLECT_COMPONENT_PROPERTY(TextResourceComponent::m_AsciiShift, "AsciiShift")
		REFLECT_COMPONENT_PROPERTY(TextResourceComponent::m_TextureAtlas, "TextureAtlas") 
		REFLECT_COMPONENT_PROPERTY(TextResourceComponent::m_CharacterPoolSize, "CharacterPoolSize")
	REFLECT_COMPONENT_END(TextResourceComponent)

	// Runtime data

	REFLECT_COMPONENT_BEGIN_DEFAULT(BufferComponent)
	REFLECT_COMPONENT_END(BufferComponent)

	REFLECT_COMPONENT_BEGIN_DEFAULT(ShaderComponent)
	REFLECT_COMPONENT_END(ShaderComponent)

	REFLECT_COMPONENT_BEGIN_DEFAULT(TextureComponent)
	REFLECT_COMPONENT_END(TextureComponent)

	REFLECT_COMPONENT_BEGIN_DEFAULT(TextComponent)
	REFLECT_COMPONENT_END(TextComponent)
} // namespace ecs
