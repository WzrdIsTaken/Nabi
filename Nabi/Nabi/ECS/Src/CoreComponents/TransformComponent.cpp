#include "Core.h"

#include "CoreComponents\TransformComponent.h"

namespace ecs
{
	REFLECT_COMPONENT_BEGIN_DEFAULT(TransformComponent)
		REFLECT_COMPONENT_PROPERTY(TransformComponent::m_Position, "Position")
		REFLECT_COMPONENT_PROPERTY(TransformComponent::m_Rotation, "Rotation")
		REFLECT_COMPONENT_PROPERTY(TransformComponent::m_Scale, "Scale")
	REFLECT_COMPONENT_END(TransformComponent)
} // namespace ecs
