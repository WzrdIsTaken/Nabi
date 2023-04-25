#pragma once
#include "Core.h"

#include "EntityGroup.h"

namespace ecs::EntityModule
{
	entt::entity FindFirstEntityByName(nabi::Context const& context, entt::hashed_string const entityName);
	nabi::Reflection::EntityGroup FindEntitiesByName(nabi::Context const& context, entt::hashed_string entityName);

	entt::entity FindFirstEntityByGroup(nabi::Context const& context, entt::hashed_string const entityGroupName);
	nabi::Reflection::EntityGroup FindEntitiesByGroup(nabi::Context const& context, entt::hashed_string const entityGroupName);

	void ForeachEntityChild(nabi::Context& context, entt::entity const entity, std::function<bool(entt::entity const)> const& action);
	void ForeachEntityChild(nabi::Context& context, std::vector<entt::entity> const& children, std::function<bool(entt::entity const)> const& action);
} // namespace ecs::EntityModule
