#include "Core.h"

#include "CoreComponents\ModelComponent.h"

namespace ecs
{
	REFLECT_COMPONENT_BEGIN_DEFAULT(ModelComponent)
		REFLECT_COMPONENT_PROPERTY(ModelComponent::m_MeshPath, "MeshPath")
		REFLECT_COMPONENT_PROPERTY(ModelComponent::m_PixelShaderPath, "PixelShaderPath")
		REFLECT_COMPONENT_PROPERTY(ModelComponent::m_VertexShaderPath, "VertexShaderPath")
		REFLECT_COMPONENT_PROPERTY(ModelComponent::m_TexturePath, "TexturePath")
	REFLECT_COMPONENT_END(ModelComponent)

	REFLECT_COMPONENT_BEGIN_DEFAULT(MeshComponent)
		REFLECT_COMPONENT_PROPERTY(MeshComponent::m_MeshId, "MeshId")
	REFLECT_COMPONENT_END(MeshComponent)

	REFLECT_COMPONENT_BEGIN_DEFAULT(ShaderComponent)
		REFLECT_COMPONENT_PROPERTY(ShaderComponent::m_VertexShaderId, "VertexShaderId")
		REFLECT_COMPONENT_PROPERTY(ShaderComponent::m_PixelShaderId, "PixelShaderId")
	REFLECT_COMPONENT_END(ShaderComponent)

	REFLECT_COMPONENT_BEGIN_DEFAULT(TextureComponent)
		REFLECT_COMPONENT_PROPERTY(TextureComponent::m_TextureId, "TextureId")
	REFLECT_COMPONENT_END(TextureComponent)
} // namespace ecs
