#include "Core.h"

#include "CoreComponents\CameraComponent.h"

namespace ecs
{
	REFLECT_COMPONENT_BEGIN_DEFAULT(CameraComponent)
		REFLECT_COMPONENT_PROPERTY(CameraComponent::m_Position, "Position")
		REFLECT_COMPONENT_PROPERTY(CameraComponent::m_Target, "Target")
	REFLECT_COMPONENT_END(CameraComponent)
} // namespace ecs
