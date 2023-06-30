#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "Demo\ECS\Components\PlayerComponent.h"

namespace ecs
{
	REFLECT_COMPONENT_BEGIN_DEFAULT(PlayerComponent)
		REFLECT_COMPONENT_PROPERTY(PlayerComponent::m_ForwardKey, "ForwardKey")
		REFLECT_COMPONENT_PROPERTY(PlayerComponent::m_BackwardKey, "BackwardKey")
		REFLECT_COMPONENT_PROPERTY(PlayerComponent::m_LeftKey, "LightKey")
		REFLECT_COMPONENT_PROPERTY(PlayerComponent::m_RightKey, "RightKey")
		REFLECT_COMPONENT_PROPERTY(PlayerComponent::m_UpKey, "UpKey")
		REFLECT_COMPONENT_PROPERTY(PlayerComponent::m_DownKey, "DownKey")
		REFLECT_COMPONENT_PROPERTY(PlayerComponent::m_Speed, "Speed")
	REFLECT_COMPONENT_END(PlayerComponent)
} // namespace ecs

#endif // ifdef INCLUDE_DEMO
