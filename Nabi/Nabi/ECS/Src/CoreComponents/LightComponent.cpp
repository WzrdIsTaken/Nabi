#include "Core.h"

#include "CoreComponents\LightComponent.h"

namespace ecs
{
	REFLECT_COMPONENT_BEGIN_DEFAULT(DirectionalLightComponent)
		REFLECT_COMPONENT_PROPERTY(DirectionalLightComponent::m_Direction, "Position")
		REFLECT_COMPONENT_PROPERTY(DirectionalLightComponent::m_Colour, "Target")
		REFLECT_COMPONENT_PROPERTY(DirectionalLightComponent::m_Intensity, "Intensity")
	REFLECT_COMPONENT_END(DirectionalLightComponent)
} // namespace ecs
