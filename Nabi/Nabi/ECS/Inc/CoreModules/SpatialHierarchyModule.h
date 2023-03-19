#pragma once
#include "Core.h"

namespace ecs::SpatialHierarchyModule
{
	void ForeachEntityChild(entt::registry& registry, entt::entity const entity, std::function<bool(entt::entity const)> const& action);
	void ForeachEntityChild(entt::registry& registry, std::vector<entt::entity> const& children, std::function<bool(entt::entity const)> const& action);
} // namespace ecs::SpatialHierarchyModule
