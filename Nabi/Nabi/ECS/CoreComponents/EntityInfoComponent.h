#pragma once

//#include "../../Libraries/entt/entt.h"

#include "../ComponentBase.h"

// Contains some basic infomation about an entity.
// No need to reflect, as its always added anyway

namespace EntityInfoComponent
{
	// Settings
	struct EntityInfoComponentSettings : nabi::ECS::ComponentSettingsBase
	{
		entt::hashed_string m_EntityGroup;
		entt::hashed_string m_EntityName;
	};

	// Component
	struct EntityInfoComponent : nabi::ECS::ComponentBase
	{
		entt::hashed_string m_EntityGroup;
		entt::hashed_string m_EntityName;

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

	private:
		EntityInfoComponent(EntityInfoComponent const&) = delete;
		EntityInfoComponent& operator = (EntityInfoComponent const&) = delete;
	};

	// Default Settings
	EntityInfoComponentSettings const entityInfoComponentDefaultSettings
	{
		.m_EntityGroup = entt::hashed_string(),
		.m_EntityName = entt::hashed_string()
	};

	// Helper
	inline EntityInfoComponentSettings ConstructSettingsFromComponent(EntityInfoComponent const& component)
	{
		EntityInfoComponentSettings settings = entityInfoComponentDefaultSettings;
		settings.m_EntityGroup = component.m_EntityGroup;
		settings.m_EntityName = component.m_EntityName;

		return settings;
	}
} // namespace EntityInfoComponent
