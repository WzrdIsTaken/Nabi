#pragma once

#include "../ComponentBase.h"

// Contains some basic infomation about an entity.
// No need to reflect, as its always added anyway

namespace ecs
{
	// Settings
	struct EntityInfoComponentSettings final : public nabi::ECS::ComponentSettingsBase
	{
		entt::hashed_string m_EntityGroup;
		entt::hashed_string m_EntityName;
	};

	// Default Settings
	EntityInfoComponentSettings const entityInfoComponentDefaultSettings
	{
		.m_EntityGroup = entt::hashed_string(),
		.m_EntityName = entt::hashed_string()
	};

	// Component
	struct EntityInfoComponent final : public nabi::ECS::ComponentBase
	{
		EntityInfoComponent()
			: m_EntityGroup(entt::hashed_string())
			, m_EntityName(entt::hashed_string())
		{
		}

		explicit EntityInfoComponent(EntityInfoComponentSettings const& settings)
			: m_EntityGroup(settings.m_EntityGroup)
			, m_EntityName(settings.m_EntityName)
		{
		}

		entt::hashed_string m_EntityGroup;
		entt::hashed_string m_EntityName;
	};
} // namespace ecs
