#include "../../../Core/EngineCore.h"

#include "../../Inc/CoreComponents/EntityInfoComponent.h"

namespace ecs
{
	EntityInfoComponent::EntityInfoComponent() NABI_NOEXCEPT
		: m_EntityGroup(entt::hashed_string())
		, m_EntityName(entt::hashed_string())
	{
	}

	EntityInfoComponent::EntityInfoComponent(EntityInfoComponentSettings const& settings) NABI_NOEXCEPT
		: m_EntityGroup(settings.m_EntityGroup)
		, m_EntityName(settings.m_EntityName)
	{
	}
} // namespace ecs
