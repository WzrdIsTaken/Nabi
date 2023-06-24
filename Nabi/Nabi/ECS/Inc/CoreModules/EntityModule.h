#pragma once
#include "Core.h"

#include "EntityGroup.h"

namespace ecs::EntityModule
{
	[[nodiscard]] entt::entity FindFirstEntityByName(nabi::Context const& context, entt::hashed_string const entityName);
	[[nodiscard]] nabi::ECS::EntityGroup FindEntitiesByName(nabi::Context const& context, entt::hashed_string entityName);

	[[nodiscard]] entt::entity FindFirstEntityByGroup(nabi::Context const& context, entt::hashed_string const entityGroupName);
	[[nodiscard]] nabi::ECS::EntityGroup FindEntitiesByGroup(nabi::Context const& context, entt::hashed_string const entityGroupName);

	void ForeachEntityChild(nabi::Context& context, entt::entity const entity, std::function<bool(entt::entity const)> const& action);
	void ForeachEntityChild(nabi::Context& context, std::vector<entt::entity> const& children, std::function<bool(entt::entity const)> const& action);
} // namespace ecs::EntityModule
