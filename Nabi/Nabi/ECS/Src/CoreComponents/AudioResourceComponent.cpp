#include "Core.h"

#include "CoreComponents\AudioResourceComponent.h"

namespace ecs
{
	REFLECT_CONTAINER(m_Resources, AudioResourceComponent)

	REFLECT_COMPONENT_BEGIN_DEFAULT(AudioResourceComponent)
		REFLECT_COMPONENT_PROPERTY(AudioResourceComponent::m_Resources, "Resources")
	REFLECT_COMPONENT_END(AudioResourceComponent)
} // namespace ecs
