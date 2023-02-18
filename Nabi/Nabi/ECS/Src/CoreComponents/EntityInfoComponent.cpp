#include "Core.h"

#include "CoreComponents/EntityInfoComponent.h"

namespace ecs
{
	EntityInfoComponent::EntityInfoComponent()
		: m_EntityGroup(entt::hashed_string())
		, m_EntityName(entt::hashed_string())
	{
	}

	EntityInfoComponent::EntityInfoComponent(EntityInfoComponentSettings const& settings)
		: m_EntityGroup(settings.m_EntityGroup)
		, m_EntityName(settings.m_EntityName)
	{
	}
} // namespace ecs
